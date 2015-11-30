/* ----------------------------- MNI Header -----------------------------------
@NAME       : register_resample
@INPUT      : argc   - number of arguments
              argv   - arguments
@OUTPUT     : (none)
@RETURNS    : (nothing)
@DESCRIPTION: Program to be invoked by register to do resampling.
              Calls mincresample with appropriate arguments.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      :
@CREATED    : March 4, 1993 (Peter Neelin) 
@MODIFIED   : $Log: register_resample.c,v $
@MODIFIED   : Revision 1.1  1999-06-21 20:21:48  stever
@MODIFIED   : .
@MODIFIED   :
 * Revision 1.19  1996/06/20  12:21:30  neelin
 * Added call to miexpand_file to only get header if file is compressed.
 *
 * Revision 1.18  1996/06/20  11:55:50  neelin
 * Got rid of lint messages for irix 5.
 *
 * Revision 1.17  1996/06/20  11:53:11  neelin
 * *** empty log message ***
 *
 * Revision 1.16  95/01/05  13:07:34  neelin
 * Added code to calculate number of resampled slices properly for pet
 * resampling (use general transform stuff from volume_io).
 * 
 * Revision 1.15  94/04/22  10:35:47  neelin
 * Removed unneeded variables.
 * 
 * Revision 1.13  94/03/25  12:31:34  neelin
 * Added ParseArgv and changed output step size algorithm (included ratio).
 * Also fixed calculation of output slices to take account of 2 volume extents.
 * 
 * Revision 1.12  93/11/03  12:41:45  neelin
 * Added miopen, miclose.
 * 
 * Revision 1.11  93/11/02  15:57:59  neelin
 * Changes for sun.
 * 
 * Revision 1.10  93/10/25  16:35:30  neelin
 * Fixed calculation of XY sampling for unusual step sizes.
 * 
 * Revision 1.9  93/08/25  10:53:50  neelin
 * Check for volume 1 being spect (as well as pet) for special resampling.
 *
---------------------------------------------------------------------------- */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <minc.h>
#include <volume_io.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ParseArgv.h>


/* Define constants */

#ifndef TRUE
#  define TRUE 1
#  define FALSE 0
#endif
#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS 0
#  define EXIT_FAILURE 1
#endif
#define LEN_DOUBLE_STRING 30
#define X 0
#define Y 1
#define Z 2
#define WORLD_NDIMS 3
static char *dimnames[] = {MIxspace, MIyspace, MIzspace};
#define RESAMPLING_PROGRAM "mincresample"
/* The scaling factor for increments in step size */
#define FACTOR 0.5
/* The maximum ratio of step size differences allowed before decreasing out
   step size: (out-v2)/(v2-out/2) */
#define RATIO 3.0

/* Types */
typedef enum {unknown, pet, mri, spect} Modality;
typedef struct {
   double start[WORLD_NDIMS];
   double step[WORLD_NDIMS];
   long size[WORLD_NDIMS];
   int dim_index[WORLD_NDIMS];
   Modality modality;
} FileInfo;

/* Function prototypes */
void get_file_info(char *filename, FileInfo *file);
void calc_pet_resample(FileInfo *vol1, FileInfo *vol2, FileInfo *out,
                       VIO_General_transform *the_transform);
char *dtoa(double dval);
char *ltoa(long lval);

/* Globals */
char *pname;
nc_type datatype = NC_BYTE;
int clobber = TRUE;
int printonly = FALSE;

/* Argument table */
ArgvInfo argTable[] = {
   {"-byte", ARGV_CONSTANT, (char *) NC_BYTE, (char *) &datatype,
       "Write out byte data (default)."},
   {"-short", ARGV_CONSTANT, (char *) NC_SHORT, (char *) &datatype,
       "Write out short data."},
   {"-long", ARGV_CONSTANT, (char *) NC_LONG, (char *) &datatype,
       "Write out long data."},
   {"-float", ARGV_CONSTANT, (char *) NC_FLOAT, (char *) &datatype,
       "Write out float data."},
   {"-double", ARGV_CONSTANT, (char *) NC_DOUBLE, (char *) &datatype,
       "Write out double data."},
   {"-filetype", ARGV_CONSTANT, (char *) NC_UNSPECIFIED, (char *) &datatype,
       "Write out data in the type of the original file."},
   {"-clobber", ARGV_CONSTANT, (char *) TRUE, (char *) &clobber,
       "Overwrite any existing file (default)."},
   {"-noclobber", ARGV_CONSTANT, (char *) FALSE, (char *) &clobber,
       "Do not overwrite any existing file."},
   {"-execute", ARGV_CONSTANT, (char *) FALSE, (char *) &printonly,
       "Execute the command to resample data (default)."},
   {"-printonly", ARGV_CONSTANT, (char *) TRUE, (char *) &printonly,
       "Print out the command to run rather than executing it."},
   {(char *) NULL, ARGV_END, (char *) NULL, (char *) NULL,
       (char *) NULL}
};

/* Main program */

int main(int argc, char *argv[])
{
   char *volume1, *volume2, *transform, *output;
   FileInfo vol1, vol2, out;
   int do_pet_resample;
   char *nargv[100];
   int nargc, iarg;
   VIO_General_transform the_transform;

   /* Check for the arguments */
   pname = argv[0];
   if (ParseArgv(&argc, argv, argTable, 0) || (argc != 5)) {
      (void) fprintf(stderr, 
         "Usage: %s [options] <volume1> <volume2> <transform> <output>\n",
                     pname);
      (void) fprintf(stderr, 
         "       %s -help\n",
                     pname);
      return EXIT_FAILURE;
   }
   volume1 = argv[1];
   volume2 = argv[2];
   transform = argv[3];
   output = argv[4];

   /* Get information from volume 1 */
   get_file_info(volume1, &vol1);

   /* Get information from volume 2 */
   get_file_info(volume2, &vol2);

   /* Set flag indicating whether we do pet resampling or not. */
   do_pet_resample = (((vol1.modality == pet) || 
                       (vol1.modality == spect)) &&
                      (vol2.modality == mri));

#if 0
   /* Old code for ensuring that we have a transverse volume */
                      (vol1.dim_index[Z] < vol1.dim_index[X]) &&
                      (vol1.dim_index[Z] < vol1.dim_index[Y]));
#endif

   /* Calculate info for new volume, if needed */
   if (do_pet_resample) {
      if (input_transform_file(transform, &the_transform) != VIO_OK) {
         (void) fprintf(stderr, 
                        "%s: Error reading transform file \"%s\".\n",
                        pname, transform);
         exit(EXIT_FAILURE);
      }
      calc_pet_resample(&vol1, &vol2, &out, &the_transform);
   }

   /* Set up argv for program invocation. First the basic stuff. */
   nargc = 0;
   nargv[nargc++] = RESAMPLING_PROGRAM;
   nargv[nargc++] = volume2;
   nargv[nargc++] = output;
   nargv[nargc++] = "-transform";
   nargv[nargc++] = transform;
   nargv[nargc++] = "-like";
   nargv[nargc++] = volume1;

   /* Next, stuff for pet resampling */
   if (do_pet_resample) {
      nargv[nargc++] = "-step";
      nargv[nargc++] = dtoa(out.step[0]);
      nargv[nargc++] = dtoa(out.step[1]);
      nargv[nargc++] = dtoa(out.step[2]);
      nargv[nargc++] = "-start";
      nargv[nargc++] = dtoa(out.start[0]);
      nargv[nargc++] = dtoa(out.start[1]);
      nargv[nargc++] = dtoa(out.start[2]);
      nargv[nargc++] = "-nelements";
      nargv[nargc++] = ltoa(out.size[0]);
      nargv[nargc++] = ltoa(out.size[1]);
      nargv[nargc++] = ltoa(out.size[2]);
   }

   /* Next clobber string */
   if (clobber) {
      nargv[nargc++] = "-clobber";
   }
   else {
      nargv[nargc++] = "-noclobber";
   }

   /* Next the type string */
   switch (datatype) {
   case NC_BYTE: nargv[nargc++] = "-byte"; break;
   case NC_SHORT: nargv[nargc++] = "-short"; break;
   case NC_LONG: nargv[nargc++] = "-long"; break;
   case NC_FLOAT: nargv[nargc++] = "-float"; break;
   case NC_DOUBLE: nargv[nargc++] = "-double"; break;
   }
   nargv[nargc++] = NULL;

   /* Invoke mincresample to do the resampling, unless printonly is set */
   if (printonly) {
      for (iarg=0; iarg < nargc-1; iarg++) {
         if (iarg > 0) {
            (void) printf(" ");
         }
         (void) printf("%s", nargv[iarg]);
      }
      (void) printf("\n");
   }
   else {

      /* Set input to /dev/null */
      (void) dup2(open("/dev/null", O_RDONLY), STDIN_FILENO);

      /* Execute mincresample */
      (void) execvp(RESAMPLING_PROGRAM, nargv);

   }

   return EXIT_SUCCESS;
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : get_file_info
@INPUT      : filename - name of file to read
@OUTPUT     : file - information about file
@RETURNS    : (nothing)
@DESCRIPTION: Routine to get information from a minc file.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : March 12, 1993 (Peter Neelin)
@MODIFIED   : 
---------------------------------------------------------------------------- */
void get_file_info(char *filename, FileInfo *file)
{
   int mincid, dimid, idim, jdim;
   int ndims, dim[MAX_VAR_DIMS];
   char string[MI_MAX_ATTSTR_LEN];
   char *tempfile;
   int created_tempfile;

   /* Open the file */
   tempfile = miexpand_file(filename, NULL, TRUE, &created_tempfile);
   mincid = miopen(tempfile, NC_NOWRITE);
   if (created_tempfile) {
      (void) remove(tempfile);
   }
   FREE(tempfile);

   /* Get dimension info */
   set_ncopts(0);
   (void) ncvarinq(mincid, ncvarid(mincid, MIimage), NULL, NULL, 
                   &ndims, dim, NULL);
   for (idim = 0; idim < WORLD_NDIMS; idim++) {
      file->start[idim] = 0.0;
      file->step[idim] = 1.0;
      dimid = ncdimid(mincid, dimnames[idim]);
      for (jdim=0 ; jdim< ndims; jdim++) {
         if (dimid == dim[jdim]) {
            file->dim_index[idim] = jdim;
            break;
         }
      }
      if ((jdim >= ndims) || 
          (ncdiminq(mincid, dimid, NULL, &file->size[idim]) == MI_ERROR)) {
         (void) fprintf(stderr, 
                        "%s: Dimension %s not used in volume %s.\n",
                        pname, dimnames[idim], filename);
         exit(EXIT_FAILURE);
      }
      (void) miattget1(mincid, ncvarid(mincid, dimnames[idim]), MIstart,
                       NC_DOUBLE, &file->start[idim]);
      (void) miattget1(mincid, ncvarid(mincid, dimnames[idim]), MIstep,
                       NC_DOUBLE, &file->step[idim]);
   }

   /* Get modality info */
   string[0] = '\0';
   (void) miattgetstr(mincid, ncvarid(mincid, MIstudy), MImodality,
                      MI_MAX_ATTSTR_LEN, string);
   if (strcmp(string, MI_PET) == 0)
      file->modality = pet;
   else if (strcmp(string, MI_MRI) == 0)
      file->modality = mri;
   else if (strcmp(string, MI_SPECT) == 0)
      file->modality = spect;
   else
      file->modality = unknown;
   set_ncopts(NC_VERBOSE | NC_FATAL);
   (void) miclose(mincid);

   return;
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : calc_pet_resample
@INPUT      : vol1 - info for volume 1
              vol2 - info for volume 2
              the_transform - transformation from volume 2 to volume 1
@OUTPUT     : out - info for output volume
@RETURNS    : (nothing)
@DESCRIPTION: Routine to calculate resampling info for pet/mri registration.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : March 12, 1993 (Peter Neelin)
@MODIFIED   : 
---------------------------------------------------------------------------- */
void calc_pet_resample(FileInfo *vol1, FileInfo *vol2, FileInfo *out,
                       VIO_General_transform *the_transform)
{
   int idim, icol, irow, islc;
   VIO_Real bottom[WORLD_NDIMS], top[WORLD_NDIMS], coord[WORLD_NDIMS];
   double diff, height;


   /* Figure out order of world axes */
   icol = VIO_X; irow = VIO_Y; islc = VIO_Z;
   if (vol1->dim_index[islc] > vol1->dim_index[irow]) {
      idim = islc; islc = irow; irow = idim;
   }
   if (vol1->dim_index[irow] > vol1->dim_index[icol]) {
      idim = irow; irow = icol; icol = idim;
   }
   if (vol1->dim_index[islc] > vol1->dim_index[irow]) {
      idim = islc; islc = irow; irow = idim;
   }

   /* For column and row start with volume 1 step size, then adjust it by 
      factors of two to get as close as possible to volume 2 */
   for (idim=0; idim < WORLD_NDIMS; idim++) {
      if (idim != islc) {
         out->step[idim] = vol1->step[idim];
         if (fabs(vol2->step[idim]) < fabs(out->step[idim])) {
            while ((fabs(vol2->step[idim]) - fabs(out->step[idim]*FACTOR)) 
                   < 0.0)
               out->step[idim] *= FACTOR;
            if (fabs(fabs(vol2->step[idim]) - fabs(out->step[idim])) > 
                RATIO*fabs(fabs(vol2->step[idim]) - 
                           fabs(out->step[idim]*FACTOR)))
               out->step[idim] *= FACTOR;
         }
      }
   }
   
   /* Make sure that aspect ratio of volume 1 is preserved (keep the smaller
      step size of the output volume) */
   if (fabs(out->step[icol]) < fabs(out->step[irow]))
      out->step[irow] = fabs(out->step[icol] / vol1->step[icol]) * 
         vol1->step[irow];
   else
      out->step[icol] = fabs(out->step[irow] / vol1->step[irow]) * 
         vol1->step[icol];
   
   /* Set the start and dimension size for row and column */
   for (idim=0; idim < WORLD_NDIMS; idim++) {
      if (idim != islc) {
         out->size[idim] = 
            (long) rint(vol1->step[idim] / out->step[idim] * 
                        vol1->size[idim]);
         out->start[idim] = 
            vol1->start[idim] + (out->step[idim] - vol1->step[idim])/2.0;
      }
   }
   
   /* Align first slice of two volumes */
   out->start[islc] = vol1->start[islc];
   
   /* Take the smaller sampling size of volumes 1 and 2 */
   if (fabs(vol2->step[islc]) < fabs(vol1->step[islc])) {
      out->step[islc] = vol2->step[islc];
   }
   else {
      out->step[islc] = vol1->step[islc];
   }

   /* Make sure that output step size in VIO_Z has the same sign as vol1 */
   if ((out->step[islc]*vol1->step[islc]) < 0.0) {
      out->step[islc] *= -1.0;
   }

   /* Set number of slices to cover full range of volume 1 or 2 */
   for (idim=0; idim < WORLD_NDIMS; idim++) {
      coord[idim] = vol2->start[idim];
      if (idim != islc) 
         coord[idim] += vol2->step[idim] * (vol2->size[idim] - 1) / 2.0;
   }
   general_transform_point(the_transform, coord[VIO_X], coord[VIO_Y], coord[VIO_Z],
                           &bottom[VIO_X], &bottom[VIO_Y], &bottom[VIO_Z]);
   coord[islc] = vol2->start[islc] + vol2->step[islc] * (vol2->size[islc] - 1);
   general_transform_point(the_transform, coord[VIO_X], coord[VIO_Y], coord[VIO_Z],
                           &top[VIO_X], &top[VIO_Y], &top[VIO_Z]);
   height = 0.0;
   for (idim=0; idim < WORLD_NDIMS; idim++) {
      diff = top[idim] - bottom[idim];
      height += diff * diff;
   }
   height = fabs((top[islc]+bottom[islc])/2.0 - out->start[islc]
      + copysign(sqrt(height)/2.0, out->step[islc]));
   if (fabs(vol1->step[islc] * (vol1->size[islc]-1)) > height) {
      out->size[islc] = (long)
         rint((vol1->size[islc]-1) * 
              fabs(vol1->step[islc] / out->step[islc]) + 1.0);
   }
   else {
      out->size[islc] = (long)
         rint(height / fabs(out->step[islc]) + 1.0);
   }

   /* Set other stuff */
   for (idim=0; idim < WORLD_NDIMS; idim++) 
      out->dim_index[idim] = vol1->dim_index[idim];
   out->modality = vol2->modality;

   return;
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : dtoa
@INPUT      : dval - double precision value
@OUTPUT     : (none)
@RETURNS    : pointer to string (must be freed by caller).
@DESCRIPTION: Converts a double to a string. Space for the string is 
              allocated by this routine and must be freed by the caller.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : March 4, 1993 (Peter Neelin)
@MODIFIED   : 
---------------------------------------------------------------------------- */
char *dtoa(double dval)
{
   char *str;

   str = malloc(LEN_DOUBLE_STRING);
   (void) sprintf(str, "%g", dval);

   return str;
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : ltoa
@INPUT      : lval - long integer value
@OUTPUT     : (none)
@RETURNS    : pointer to string (must be freed by caller).
@DESCRIPTION: Converts a long to a string. Space for the string is 
              allocated by this routine and must be freed by the caller.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : March 4, 1993 (Peter Neelin)
@MODIFIED   : 
---------------------------------------------------------------------------- */
char *ltoa(long lval)
{
   char *str;

   str = malloc(LEN_DOUBLE_STRING);
   (void) sprintf(str, "%d", (int) lval);

   return str;
}

