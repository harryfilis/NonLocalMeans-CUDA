#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

//#include "G:\Program Files\MATLAB\R2018a\extern\include\mat.h"

// All matrices are in row major format

// float *F = matOpen("../code/data/house.mat",)


/*Returns the values of a patch given the central pixel positions in the image*/
float* patch_finder(float *padded_array, int patch_size, int row_central, int col_central, int rows, int cols){

    if (row_central<0 || row_central>rows-1 || col_central<0 || col_central>cols-1){
        printf("Invalid centre for patch.\n");
        exit(-1);
    }

    // Allocate space for the patch
    float* Patch = (float *)malloc(patch_size*patch_size* sizeof(float));
    
    // Useful for changing line in the padded array
    int padded_cols = cols + patch_size - 1;

    // Stores the index of the first element of the patch in the padded aray
    int first_elem = row_central*padded_cols + col_central;

    for (int i = 0; i < patch_size; i++){
        for (int j = 0; j < patch_size; j++){
            Patch[i*patch_size + j] = padded_array[first_elem + i*padded_cols + j];
        }
    }

    return Patch;
}


//ARRAY PADDING 
//extends F by patch_size / 2 from each side and uses the mirroring technique
float* pad_array(float *F, int rows, int cols, int patch_size){

    int rows_new = rows + patch_size - 1;
    int cols_new = cols + patch_size - 1;

    // The extended array to be returned
    float *pad_array = (float*)malloc(rows_new*cols_new*sizeof(float));

    int extension = patch_size/2;

    float *upper_extension = (float*)malloc(extension*cols*sizeof(float));

    int index = 0;
    for (int i=extension-1; i>-1; i--){
        for (int j=0; j<cols; j++){
            upper_extension[i*cols+j] = F[index];
            index++;

        }
    }

    float *lower_extension = (float*)malloc(extension*cols*sizeof(float));
    // Start from the last the element 
    index = rows*cols-1;
    for (int i=0; i<extension; i++){
        for (int j= cols - 1; j>-1; j--){
            lower_extension[i*cols+j] = F[index];
            index--;
        }
    }
    /*
        Upper left and Right and Lower left and Right extensions of F are symmmetrical to upper_extension and lower_extension respectively.    
    */
    for (int i = 0; i < rows_new; i++){
        for (int j = 0; j < cols_new; j++){
            //For the left side
            if (j < extension){
                // Upper left
                if (i < extension){
                    pad_array[i*cols_new + j] = upper_extension[i*cols + extension - 1 - j];
                }
                // Middle left
                else if (i >= extension && i < rows + extension){
                    pad_array[i*cols_new + j] = F[(i-extension)*cols + extension - 1 - j];
                }
                // Bottom left
                else{
                    pad_array[i*cols_new +j] = lower_extension[(i-extension-rows)*cols + extension - 1 - j];
                }
                
            }
            // For the middle cols
            else if (j >= extension  && j < extension + cols){ 
                int new_j = j -extension;
                // Upper
                if (i < extension){ 
                    pad_array[i*cols_new + j] = upper_extension[i*cols + new_j];
                }
                // Middle
                else if (i >= extension && i < rows + extension){
                    pad_array[i*cols_new + j] = F[(i-extension)*cols + new_j];
                }
                // Bottom
                else{
                    pad_array[i*cols_new + j] = lower_extension[(i-extension-rows)*cols + new_j];
                }
            }
            // For the right side
            else{
                int new_j = j -extension - cols + 1;
                // Upper right
                if (i < extension){ 
                    pad_array[i*cols_new + j] = upper_extension[i*cols + cols - new_j];
                }
                // Middle right
                else if (i >= extension && i < rows + extension){
                    pad_array[i*cols_new + j] = F[(i-extension)*cols + cols - new_j];
                }
                // Bottom right
                else{
                    pad_array[i*cols_new + j] = lower_extension[(i-extension-rows)*cols + cols - new_j];
                }
            }
        }
    }
    

    free(upper_extension);
    free(lower_extension);

    return pad_array;
}



float *matToRowMajor(float** matrix, int n, int m){
        float *RowMajor = (float*)malloc(n*m*sizeof(float));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                RowMajor[i*n+j] = matrix[i][j];
            }
            
        }
        
        return RowMajor; 
}
 
float* read_csv(int row, int col, char *filename){
	FILE *file;
	file = fopen(filename, "r");
    if (file == NULL)
    {
       fprintf(stderr,"error fopen(): Failed to open CSV file");
    }
    
    float **data = (float **)malloc(row*sizeof(float*));
    for (int i = 0; i < row; i++)
    {
        data[i] = (float*)malloc(row*sizeof(float));
    }
    float *dataRowMajor = (float*)malloc(row*col*sizeof(float));
            
    int count = 0;
    char c;
    for (c = getc(file); c != EOF; c = getc(file)){ 
    // Increment count for this character 
        count = count + 1; 
    }
    char buffer[count];
    char *record, *line;
    int i=0,j=0;
    while((line=fgets(buffer,sizeof(buffer),file))!=NULL)
    {
        record = strtok(line,",");
        while(record != NULL)
        {
            printf("%s\t",record) ;    //here you can put the record into the array as per your requirement.
            printf("\n");
            data[i][j++] = atof(record);
            record = strtok(NULL,",");
            
        }
        ++i ;
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%f,", data[i][j]);
        }
        printf("\n");
    }
    
    
    dataRowMajor = matToRowMajor(data, row, col);
    for (int i = 0; i < row; i++)
    {
        free(data[i]);
    }
    free(data);
    return dataRowMajor;
    
}






// double* nonLocalMeans(float **F, int patch_size, float filter_sigma, float patch_sigma){
//     float** Patch = (float *)malloc(patch_size * sizeof(float*));
//     for (int i = 0; i <//toimport size of the image; i++)
//     {
//         Patch[i] = malloc(patch_size * sizeof(float));
//     }
    
//     Patch = patch_finder(F,patch_size);


//     //apply gaussian filter to patch 
    
//     Gaussian_Patch = gaussian_Filtering(Patch, patch_size, patch_sigma);


//     free(Patch);
    
// }


// float** gaussian_Filtering(float** P, int size, float sigma){
//     float* gauss = malloc(size*size*sizeof(float));
//     float sum = 0;
//     for (int i = 0; i < size; i++)
//     {
//         for (int j = 0; j < size; j++)
//         {
//             float rows[size] = P[]
//             float columns[size] = 
//         }
        
//     }
    
// }
// for (int i = 0; i < COLS; i++)
// {
//     for (int j = 0; j < ROWS; j++)
//     {
//         for (int j = 0; j < size; j++)
//         {
//             float rows[size] = P[]
//             float columns[size] = 
//         }
        
//     }
    
// }
// // for (int i = 0; i < COLS; i++)
// // {
// //     for (int j = 0; j < ROWS; j++)
// //     {
// //         printf("%f", a[i*COLS + j])
// //     }
    
    
// // }
