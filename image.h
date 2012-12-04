#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include "pose.h"

class Image{
public:
    Image(int width, int height, bool color = true){
        width_ = width;
        height_ = height;
        if(color){
            bytesPerPixel_ = 3;
            colorSpace_ = JCS_RGB;
        }else{
            bytesPerPixel_ = 1;
            colorSpace_ = JCS_GRAYSCALE;
        }
        data_ = (unsigned char*)malloc( width_*height_*bytesPerPixel_ *sizeof(unsigned char));
    }

    int read_jpeg_file( std::string filename )
    {
        /* these are standard libjpeg structures for reading(decompression) */
        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;
        /* libjpeg data structure for storing one row, that is, scanline of an image */
        JSAMPROW row_pointer[1];

        FILE *infile = fopen( filename.c_str(), "rb" );
        unsigned long location = 0;
        int i = 0;

        if ( !infile ){
            std::cout << "Error opening input jpeg file " << filename << "!" << std::endl;
            return -1;
        }
        /* here we set up the standard libjpeg error handler */
        cinfo.err = jpeg_std_error( &jerr );
        /* setup decompression process and source, then read JPEG header */
        jpeg_create_decompress( &cinfo );
        /* this makes the library read from infile */
        jpeg_stdio_src( &cinfo, infile );
        /* reading the image header which contains image information */
        jpeg_read_header( &cinfo, TRUE );
        /* Uncomment the following to output image information, if needed. */
        /*--
        printf( "JPEG File Information: \n" );
        printf( "Image width and height: %d pixels and %d pixels.\n", cinfo.image_width, cinfo.image_height );
        printf( "Color components per pixel: %d.\n", cinfo.num_components );
        printf( "Color space: %d.\n", cinfo.jpeg_color_space );
        --*/
        /* Start decompression jpeg here */
        jpeg_start_decompress( &cinfo );

        /* allocate memory to hold the uncompressed image */
        data_ = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
        /* now actually read the jpeg into the raw buffer */
        row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
        /* read one scan line at a time */
        while( cinfo.output_scanline < cinfo.image_height )
        {
            jpeg_read_scanlines( &cinfo, row_pointer, 1 );
            for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
                data_[location++] = row_pointer[0][i];
        }
        /* wrap up decompression, destroy objects, free pointers and close open files */
        jpeg_finish_decompress( &cinfo );
        jpeg_destroy_decompress( &cinfo );
        free( row_pointer[0] );
        fclose( infile );
        /* yup, we succeeded! */
        return 1;
    }

    int write(std::string filename){
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;

        /* this is a pointer to one row of image data */
        JSAMPROW row_pointer[1];
        FILE *outfile = fopen( filename.c_str(), "wb" );

        if ( !outfile ){
            std::cout << "Error opening output jpeg file " << filename << "!" << std::endl;
            return -1;
        }
        cinfo.err = jpeg_std_error( &jerr );
        jpeg_create_compress(&cinfo);
        jpeg_stdio_dest(&cinfo, outfile);

        /* Setting the parameters of the output file here */
        cinfo.image_width = width_;
        cinfo.image_height = height_;
        cinfo.input_components = bytesPerPixel_;
        cinfo.in_color_space = colorSpace_;
        /* default compression parameters, we shouldn't be worried about these */
        jpeg_set_defaults( &cinfo );
        /* Now do the compression .. */
        jpeg_start_compress( &cinfo, TRUE );
        /* like reading a file, this time write one row at a time */
        while( cinfo.next_scanline < cinfo.image_height )
        {
            row_pointer[0] = &data_[ cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
            jpeg_write_scanlines( &cinfo, row_pointer, 1 );
        }
        /* similar to read file, clean up after we're done compressing */
        jpeg_finish_compress( &cinfo );
        jpeg_destroy_compress( &cinfo );
        fclose( outfile );
        /* success code is 1! */
        return 1;
    }

    int loadFromData(unsigned char* data, int width, int height, int bytesPerPixel){
            struct jpeg_decompress_struct cinfo;
            struct jpeg_error_mgr jerr;
            /* libjpeg data structure for storing one row, that is, scanline of an image */
            JSAMPROW row_pointer[1];

            /* here we set up the standard libjpeg error handler */
            cinfo.err = jpeg_std_error( &jerr );
            /* setup decompression process and source, then read JPEG header */
            jpeg_create_decompress( &cinfo );
            /* this makes the library read from infile */
            jpeg_mem_src( &cinfo, data, width*height*3 );
            /* reading the image header which contains image information */
            jpeg_read_header( &cinfo, TRUE );
            /* Uncomment the following to output image information, if needed. */
            /*--
            printf( "JPEG File Information: \n" );
            printf( "Image width and height: %d pixels and %d pixels.\n", cinfo.image_width, cinfo.image_height );
            printf( "Color components per pixel: %d.\n", cinfo.num_components );
            printf( "Color space: %d.\n", cinfo.jpeg_color_space );
            --*/
            /* Start decompression jpeg here */
            jpeg_start_decompress( &cinfo );

            /* allocate memory to hold the uncompressed image */
            data_ = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
            /* now actually read the jpeg into the raw buffer */
            row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
            /* read one scan line at a time */
            unsigned long location = 0;
            while( cinfo.output_scanline < cinfo.image_height )
            {
                jpeg_read_scanlines( &cinfo, row_pointer, 1 );
                for(int i=0; i<cinfo.image_width*cinfo.num_components;i++)
                    data_[location++] = row_pointer[0][i];
            }
            /* wrap up decompression, destroy objects, free pointers and close open files */
            jpeg_finish_decompress( &cinfo );
            jpeg_destroy_decompress( &cinfo );
            free( row_pointer[0] );

            return 1;
    }

    void add(Image img, Pose pose){

    }

    void crop(int x0, int y0, int width, int height){

    }

    unsigned char *data_;

private:
    int bytesPerPixel_;
    J_COLOR_SPACE colorSpace_;
    int width_;
    int height_;

};

#endif //IMAGE_H
