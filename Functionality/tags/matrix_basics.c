/* ----------------------------- MNI Header -----------------------------------
@NAME       : matrix_basics.c
@DESCRIPTION: File containing routines for doing basic matrix calculations
@METHOD     : Contains routines :
                 printmatrix
                 calc_centroid
                 translate
                 transpose
                 matrix_multiply
                 trace
                 matrix_scalar_multiply
@CALLS      : 
@CREATED    : January 31, 1992 (Peter Neelin)
@MODIFIED   : 
---------------------------------------------------------------------------- */
#include <mni.h>
#include <recipes.h>

/* Routines defined in this file */


/* ----------------------------- MNI Header -----------------------------------
@NAME       : printmatrix
@INPUT      : rows   - number of rows in matrix
              cols   - number of columns in matrix
              the_matrix - matrix to be printed (in numerical recipes form).
                 The dimensions of this matrix should be defined to be 
                 1 to rows and 1 to cols (when calling the numerical 
                 recipes routine matrix).
@OUTPUT     : (nothing)
@RETURNS    : (nothing)
@DESCRIPTION: Prints out a matrix on stdout with one row per line.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code
---------------------------------------------------------------------------- */
public void printmatrix(int rows, int cols, float **the_matrix)
{
   int i,j;
   float f;

   /* Loop through rows and columns, printing one row per line */
   for (i=1; i <= rows; ++i) {
      for (j=1; j <= cols; ++j) {
         f=the_matrix[i][j];
         (void) printf(" %10.6f ",f);
      }
      (void) printf("\n");
   }
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : calc_centroid
@INPUT      : npoints - number of points
              ndim    - number of dimensions
              points  - points matrix (in numerical recipes form).
                 The dimensions of this matrix should be defined to be 
                 1 to npoints and 1 to ndim (when calling the numerical 
                 recipes routine matrix).
@OUTPUT     : centroid - vector of centroid of points (in num. rec. form)
                 This vector should run from 1 to ndim.
@RETURNS    : (nothing)
@DESCRIPTION: Calculates the centroid of a number of points in ndim dimensions.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code and modified calling
                 sequence.
---------------------------------------------------------------------------- */
public void calc_centroid(int npoints, int ndim, float **points, 
                          float *centroid)
{
   int i,j;

   /* Loop over each dimension */
   for (i=1; i <= ndim; ++i) {
      /* Add up points and divide by number of points */
      centroid[i] = 0;
      for (j=1; j <= npoints; ++j) {
         centroid[i] += points[j][i];
      }
      if (npoints >0) centroid[i] /= (float) npoints;
   }
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : translate_points
@INPUT      : npoints - number of points
              ndim    - number of dimensions
              points  - points matrix (in numerical recipes form).
                 The dimensions of this matrix should be defined to be 
                 1 to npoints and 1 to ndim (when calling the numerical 
                 recipes routine matrix).
              translation - translation vector (in num. rec. form, running
                 from 1 to ndim).
@OUTPUT     : newpoints - translated points matrix (see points). This matrix
                 can be the original points matrix.
@RETURNS    : (nothing)
@DESCRIPTION: Translates a set of points by a given translation.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code and modified calling
                 sequence.
---------------------------------------------------------------------------- */
public void translate_points(int npoints, int ndim, float **points, 
                      float *translation, float **newpoints)
{
   int i,j;

   for (i=1; i <= npoints; ++i) {
      for (j=1; j <= ndim; ++j) {
         newpoints[i][j] = points[i][j] + translation[j];
      }
   }
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : transpose
@INPUT      : rows    - number of rows
              cols    - number of columns
              mat     - original matrix (in numerical recipes form).
                 The dimensions of this matrix should be defined to be 
                 1 to rows and 1 to cols (when calling the numerical 
                 recipes routine matrix).
@OUTPUT     : mat_transpose  - transposed matrix (in numerical recipes form,
                 with dimensions 1 to cols and 1 to rows). Matrix 
                 mat_transpose cannot be the original matrix mat.
@RETURNS    : (nothing)
@DESCRIPTION: Transposes a matrix.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code and modified calling
                 sequence.
---------------------------------------------------------------------------- */
public void transpose(int rows, int cols, float **mat, float **mat_transpose)
{
   int i,j;

   for (i=1; i <= rows; ++i) {
      for (j=1; j <= cols; ++j) {
         mat_transpose[j][i]=mat[i][j];
      }
   }
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : raw_matrix_multiply
@INPUT      : ldim, mdim, ndim - dimensions of matrices. Matrix Amat has
                 dimensions (ldim x mdim), matrix Bmat has dimension
                 (mdim x ndim) and resultant matrix has dimension
                 (ldim x ndim).
              Amat - First matrix of multiply (in numerical recipes form).
                 Dimensions are 1 to ldim and 1 to mdim.
              Bmat - Second matrix of multiply (in numerical recipes form).
                 Dimensions are 1 to mdim and 1 to ndim.
@OUTPUT     : Cmat - Resulting matrix (in numerical recipes form).
                 Dimensions are 1 to ldim and 1 to ndim. This matrix cannot
                 be either Amat or Bmat.
@RETURNS    : (nothing)
@DESCRIPTION: Multiplies two matrices.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code and modified calling
                 sequence.
---------------------------------------------------------------------------- */
public void raw_matrix_multiply(int ldim, int mdim, int ndim, 
                                float **Amat, float **Bmat, float **Cmat)
{
   int i,j,k;

   /* Zero the output matrix */
   for (i=1; i <= ldim; ++i)
      for (j=1; j <= ndim; ++j)
         Cmat[i][j]=0.;

   /* Calculate the product */
   for (i=1; i <= ldim; ++i)
      for (j=1; j <= ndim; ++j)
         for (k=1; k <=mdim; ++k)
            Cmat[i][j] += (Amat[i][k] * Bmat[k][j]);
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : matrix_multiply
@INPUT      : ldim, mdim, ndim - dimensions of matrices. Matrix Amat has
                 dimensions (ldim x mdim), matrix Bmat has dimension
                 (mdim x ndim) and resultant matrix has dimension
                 (ldim x ndim).
              Amat - First matrix of multiply (in numerical recipes form).
                 Dimensions are 1 to ldim and 1 to mdim.
              Bmat - Second matrix of multiply (in numerical recipes form).
                 Dimensions are 1 to mdim and 1 to ndim.
@OUTPUT     : Cmat - Resulting matrix (in numerical recipes form).
                 Dimensions are 1 to ldim and 1 to ndim. This matrix can
                 be either matrix Amat or Bmat.
@RETURNS    : (nothing)
@DESCRIPTION: Multiplies two matrices.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : March 2, 1992 (Peter Neelin)
@MODIFIED   : March 2, 1992 (P.N.)
                 - Changed so that calling program can use an input matrix for
                 output.
---------------------------------------------------------------------------- */
public void matrix_multiply(int ldim, int mdim, int ndim, 
                            float **Amat, float **Bmat, float **Cmat)
{
   int i,j;
   float **Ctemp;

   /* Allocate a temporary matrix */
   Ctemp = matrix(1,ldim,1,ndim);

   /* Do the multiplication */
   raw_matrix_multiply(ldim,mdim,ndim,Amat,Bmat,Ctemp);

   /* Copy the result */
   for (i=1; i <= ldim; ++i)
      for (j=1; j <= ndim; ++j)
         Cmat[i][j] = Ctemp[i][j];

   /* Free the matrix */
   free_matrix(Ctemp,1,ldim,1,ndim);
}
                  

/* ----------------------------- MNI Header -----------------------------------
@NAME       : trace
@INPUT      : size   - size of the_matrix (the_matrix should be square)
              the_matrix - matrix for which trace should be calculated (in 
                 numerical recipes form). Dimensions are 1 to size and 
                 1 to size.
@OUTPUT     : (none)
@RETURNS    : trace of matrix
@DESCRIPTION: Calculates the trace of a matrix.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code and modified calling
                 sequence.
---------------------------------------------------------------------------- */
public float trace(int size, float **the_matrix)
{
   float sum=0.;
   int i;

   for (i=1; i <= size; ++i) {
      sum += the_matrix[i][i];
   }

   return(sum);
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : matrix_scalar_multiply
@INPUT      : rows    - number of rows of the_matrix.
              cols    - number of columns of the_matrix
              scalar  - scalar by which the_matrix should be multiplied.
              the_matrix  - matrix to be multiplied (in numerical recipes 
                 form). Dimensions are 1 to rows and 1 to cols.
@OUTPUT     : product - result of multiply ( in numerical recipes form).
                 Dimensions are 1 to rows and 1 to cols. This matrix
                 can be the input matrix.
@RETURNS    : (nothing)
@DESCRIPTION: Multiplies a matrix by a scalar.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code and modified calling
                 sequence.
---------------------------------------------------------------------------- */
public void matrix_scalar_multiply(int rows, int cols, float scalar, 
                            float **the_matrix, float **product)
{
   int i,j;

   for (i=1; i <= rows; ++i)
      for (j=1; j<=cols; ++j)
         product[i][j]=scalar*the_matrix[i][j];
}

