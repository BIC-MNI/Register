---
section: 1
title: register
author: Robert D. Vincent
---
# register

interactive volume display and point tagging program
`register OPTION volume1_file volume2_file tag_file`

## DESCRIPTION

*Register* is an interactive graphics application that can display two volumes (typically MR or PET), and the merged image of the two. There are three viewports for each volume, showing transverse, sagittal, and coronal slices. The user can move throughout the volumes, and create tag points within the volumes. If two volumes are loaded and enough tag points are created, then a transformation is computed for registering the two volumes. This transform is used to display the merged image and to allow the user to start a process which resamples one of the volumes into the same space as the other.

A byte vector volume will normally be treated as an RGB volume, in that the first three byte values will be interpreted as red, blue, and green intensities. Alternatively, the vector can be converted to a scalar whose value is the mean of the vector components, by setting the global variable *Convert_vectors_to_rgb* to FALSE.

## OPTIONS

The following command line options are recognized:

`-global NAME VALUE`
Sets the global variable *NAME* to *VALUE*.

`-sync`  
Start the program with the two volume positions synchronized.

`-range VOLUME MIN MAX`
Force the initial colour mapping range of *VOLUME* (either 0 or 1) to the values *MIN* and *MAX*.

`-version`  
Print the program version number and exit.

## USER INTERFACE

The user interface is a mouse-based interface composed of buttons, sliders, and text entry. Buttons can be pressed with the left mouse button. Text entries can be changed by pressing the left mouse button on them, then typing text. Typing return signals the text entry is done. Typing escape ends text editing without modifying the text. The delete, cursor left, and cursor right keys may be used. Ctrl-a and ctrl-e move the cursor to the beginning and end of text, respectively. Ctrl-u deletes all text in the text entry.

The up and down arrow keys navigate between the tag point fields.

The left and right arrow keys increase or decrease the displayed slice coordinate for the volume under the mouse pointer.

The '>' and '<' keys will step forward and backward in the time dimension of the volume under the mouse pointer, if possible.

The 'S' key will save the slice image under the mouse in a file named "register_image_NNN.tiff".

The 'T' key will display a window containing the graph of the timecourse of the currently selected voxel, if the current volume has a time dimension.

The 'R' and 'E' keys will move the blend slider to the right or left, respectively. This allows for changing the relative visibility of the merged volumes from the keyboard. However, it is active only with the 'Blend' merge method.

## MAIN MENU

*Quit:* Quits the program, after confirmation.

*Resample:* Resamples the second volume to place it in the same space as the first, according to the transformation defined by the tag points.

*Resample Filename:* Text entry to set the name of the file to create for the resampling.

*Not Synced / Synced:* Toggle whether the cursor position in world coordinates is shared between all volumes.

*Colour Map / RGB:* Toggles between colour map mode (faster updates) and RGB mode (truer colours).

*Double / Single:* Toggles between double and single buffer mode.

*Load Tags:* Loads a set of tag points into the program.

*Tags Filename:* Text entry to set name of filename to be used by load and save tags. A default extension ".tag" is provided automatically.

*Save Tags:* Saves all active tag points to a file. If one volume is loaded, then a set of single tags is saved. If two volumes are loaded, then a set of pairs of tags are saved.

*Save Transform:* Saves the transform defined by the tag points, if it exists.

*Transform Filename:* Text entry to set name of filename to be used by save transform. A default extension ".xfm" is provided automatically.

*Record Tag:* Overwrites the current tag point positions with the current positions within the volumes. If the current tag point is the end of the tags, then it creates a new tag point.

*Delete Tag:* Deletes the current tag point.

*Delete All Tags:* Pops up a confirmation box allowing you to delete all the tags.

*Transform Type:* Pops up a dialog to select 6 parameter, 7 parameter, 12 parameter, or thin-plate spline non-linear fitting.

*Tags Visible/Invisible* Toggles the display of tag point positions on the slices.

*Cursor Visible/Invisible:* Toggles the display of cursor crosshair on the slices.

*Interp: Flat/Smooth:* Toggles the display of slices between flat and smooth shading. Smooth shading takes much longer to update (a few seconds), but produces higher quality images, especially noticeable when the images are zoomed up.

*RMS:* Displays the average RMS error of the tag points, if a transform exists.

## VOLUME MENUS

*Voxel and World Position Readouts* Displays the position of the volume cursor in voxel and world coordinates. The positions can be changed directly by clicking on the field and typing in the desired position.

*Load Volume:* Loads the volume specified in the volume filename field.

*Volume Filename:* Specifies the volume filename.

*Reset View:* Resets the scale and translation of the slices to fit in the viewports.

*Filter* Pops up a dialog window that selects the filter type for each of the 3 views of the volume. The filtering applies across slices, but not within a slice. The filter types are: nearest neighbour, linear interpolation, box filter, triangle filter, and gaussian filter.

*Under Colour:* Sets the colour displayed for voxels whose values are less than the range.

*Over Colour:* Sets the colour displayed for voxels whose values are greater than the range.

*Colour Bar Slider:* Sets the colour range. The left mouse can be used to drag the upper or lower limit. The middle mouse is used to move both at the same time.

*Colour Coding Buttons:* Sets the colour coding method to one of gray scale, hot metal, spectral, red, green, or blue.

## MERGED MENU

*Voxel and World Position Readouts:* Displays the position of the volume cursor in voxel and world coordinates. The positions can be changed directly by clicking on the field and typing in the desired position.

*Reset View:* Resets the scale and translation of the slices to fit in the viewports.

*Merge Method Buttons:* Sets the merge method to one of blend, weighted, one on two, or two on one. Blend allows blending between the two volumes. Weighted allows arbitrary weights for the two volumes. One on two places volume one on top of volume two, with volume two showing through if volume one is out of range. Two on one is the opposite.

*Opacity Sliders* Sets the weights of the two volumes. Only used if the merge method is blend or weighted.

## TAGS MENU

*Position:* Displays tag positions. Positions can be directly edited.

*Name:* Sets the name of a tag.

*On / Ignore:* Controls whether the tag contributes to the transformation.

*Dst:* Displays the distance of the tag point in the first volume from the position of the transformed

## TIMECOURSE MENU

*Full range/Scaled:* Toggle the Y-axis range between the either the full range of the volume or scaled to the specific time course.

*T(min):* Set the minimum time displayed (typically in seconds).

*T(max)* Set the maximum time displayed.

*Save* Save the current graph's data points to an ASCII file.

*Save filename* Sets the filename which the *Save* button will use.

*Close* Closes the timecourse window.

## MOUSE AND KEYBOARD

In the slice viewports, the following actions are valid:

*Left Mouse:* Sets the volume cursor position.

*Middle Mouse:* Moves slice perpendicular according to the Y position of mouse.

*Right Mouse:* Adds a new tag point at the current volume cursor positions.

*Shift/Ctrl/Alt Left Mouse:* Translates the slice display

*Shift/Ctrl/Alt Middle Mouse:* Zooms the slice display according to the y position of the mouse.

## FILES

/usr/local/lib/register.globals

/usr/local/lib/register_UI.globals

## AUTHOR

David MacDonald

## BUGS

Sometimes turning the tags visibilities off does not turn them off.

Switching between colour map and rgb mode or double/single buffer on the OpenGL version sometimes prints error messages, or core dumps.

In smooth interpolation mode, filtering has not been implemented.

## BUG REPORTS TO

Robert D. Vincent robert.d.vincent@mcgill.ca

## COPYRIGHTS

*©*Copyright 1993-2015 by David MacDonald
