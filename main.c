/* Playing around with images and geometric shape generation */

#include <stdio.h>
#include <math.h>

#include "include/stb_image.h"
#include "include/stb_image_write.h"

#define PI 3.1415926535

#define WIDTH 500
#define HEIGHT 500

struct color
{
	int r, g, b;
};

unsigned char* data;

int get_rand_int(int lower, int upper)
{ return  (rand() % (upper - lower + 1)) + lower; }

int get_pixel_pos(int x, int y, int w)
{ return (y * w + x) * 3; }

// Checks if a point is in our circle 
int is_in_circle(int x, int y, int radius, int c_x, int c_y)
{ return (sqrt(( (abs(x - c_x) * abs(x - c_x)) + 
				 (abs(y - c_y) * abs(y - c_y))
				 )) < radius); }

// Goes through every pixel of our image and sets it to the given color 
void set_image_background(struct color color, size_t size, int channels)
{
	for (unsigned char* i = data; i != data + size; i += channels)
	{
		*(i) = color.r; 
		*(i + 1) = color.g;
		*(i + 2) = color.b;
	}
}

// Draws filled circle
void draw_circle(int c_x, int c_y, int r, struct color color)
{
	// Fills the circle up

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (is_in_circle(x, y, r, c_x, c_y))
			{
				int t_r = get_rand_int(0, 255);
				int t_g = get_rand_int(0, 255);
				int t_b = get_rand_int(0, 255);

				*(data + (get_pixel_pos(x, y, WIDTH))) = color.r;
				*(data + (get_pixel_pos(x, y, WIDTH)) + 1) = color.g;
				*(data + (get_pixel_pos(x, y, WIDTH)) + 2) = color.b;
			}
		}
	}
}

// Draws outlined circle
void draw_circle_outline(int c_x, int c_y, int r)
{
	double i = 0;
	double angle = 0;
	double x1 = 0;
	double y1 = 0;

	for (i = 0; i < 360; i+= 0.1)
	{
		angle = i;
		x1 = r * cos(angle * PI / 180);
		y1 = r * sin(angle * PI / 180);

		// Generates outline of circle
		*(data + (get_pixel_pos(c_x + x1, c_y + y1, WIDTH))) = 255;
		*(data + (get_pixel_pos(c_x + x1, c_y + y1, WIDTH)) + 1) = 0;
		*(data + (get_pixel_pos(c_x + x1, c_y + y1, WIDTH)) + 2) = 0;
	}
}

// Draws outlined rectangle
void draw_rect_outline(int x, int y, int w, int h, struct color color)
{
	// Horizontally
	for (int i = x; i < w + 1; i++)
	{
		*(data + get_pixel_pos(x + i, y, WIDTH)) = color.r;
		*((data + get_pixel_pos(x + i, y, WIDTH)) + 1) = color.g;
		*((data + get_pixel_pos(x + i, y, WIDTH)) + 2) = color.b;

		*(data + get_pixel_pos(x + i, y + h, WIDTH)) = color.r;
		*((data + get_pixel_pos(x + i, y + h, WIDTH)) + 1) = color.g;
		*((data + get_pixel_pos(x + i, y + h, WIDTH)) + 2) = color.b;
	}

	// Vertically
	for (int i = y; i < h + 1; i++)
	{
		*(data + get_pixel_pos(x, y + i, WIDTH)) = color.r;
		*((data + get_pixel_pos(x, y + i, WIDTH)) + 1) = color.g;
		*((data + get_pixel_pos(x, y + i, WIDTH)) + 2) = color.b;

		*(data + get_pixel_pos(x + w, y + i, WIDTH)) = color.r;
		*((data + get_pixel_pos(x + w, y + i, WIDTH)) + 1) = color.g;
		*((data + get_pixel_pos(x + w, y + i, WIDTH)) + 2) = color.b;
	}
}

// Checks if point is in rectangle
int is_in_rect(int px, int py, int rx, int ry, int rw, int rh)
{
	return px > rx && px < rx + rw &&
			py > ry && py < ry + rh;
}	

// Draws filled rectangle
void draw_rect(int x, int y, int w, int h, struct color color)
{
	for (int py = 0; py < HEIGHT; py++)
	{
		for (int px = 0; px < WIDTH; px++)
		{
			if (is_in_rect(px, py, x, y, w, h))
			{
				int t_r = get_rand_int(0, 255);
				int t_g = get_rand_int(0, 255);
				int t_b = get_rand_int(0, 255);

				*(data + (get_pixel_pos(px, py, WIDTH))) = color.r;
				*(data + (get_pixel_pos(px, py, WIDTH)) + 1) = color.g;
				*(data + (get_pixel_pos(px, py, WIDTH)) + 2) = color.b;
			}
		}
	}
}

// This example draws a rectangle and a circle on an image created by the program
int main(void)
{
	// Specify the amount of channels that our output image has  
	int channels = 3; // RGB
	size_t size = WIDTH * HEIGHT * channels; // Total size in bits of our image 
	data = (unsigned char*)malloc(size); // data of our image

	set_image_background((struct color){255, 0, 255}, size, channels); // We set the background as magenta 

	draw_rect(0.0f, 0.0f, 30, 30, (struct color){255.0f, 255.0f, 255.0f}); 
	draw_circle(200, 100, 50, (struct color){255.0f, 255.0f, 255.0f});

	// We use jpg because thats the format for 3 channel images 
	stbi_write_jpg("output.jpg", WIDTH, HEIGHT, channels, data, 100);

	// Free memory like a good boi 
	free(data);

	return 0;
}

