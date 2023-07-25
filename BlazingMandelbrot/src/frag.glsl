/*
	Blazing Mandelbrot Shader

	MIT License

	Copyright (c) 2021 Jessica Leyba
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
uniform float time; 
uniform vec2 resolution; 
uniform int iterations; 
uniform float zoom = 0.1; 
uniform float zoomPosX = 0.755; 
uniform float panMod = 0.3; 


// Make a complex square! 
vec2 complex_square(vec2 num)
{
	float real = num.x * num.x - num.y * num.y;
	float imaginary = num.x * num.y * 2.0; 
	return vec2(real, imaginary); 
}

// HSV to RGB  
vec3 hsv(float h, float s, float v)
{
    vec4 k = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(vec3(h) + k.xyz) * 6.0 - vec3(k.w));
    return v * mix(vec3(k.x), clamp(p - vec3(k.x), 0.0, 1.0), s);
}

void main()
{
	vec2 position = gl_FragCoord.xy / resolution * 2.0 - 1.0;
	vec2 z = vec2(sin(time * panMod / 64.0), 0.0);
	float zlength = 0.0; 
	vec2 c = position - 0.5;
	float k = 0.0;

	// Zoom 
	c.y *= -1; 
	c /= pow(1.5, zoom - 3.0);
	c.y += 0.1;
	c.x -= sqrt(zoomPosX * zoomPosX - 0.1 * 0.1);

	// Loop to draw the mandelbrot 
	for (int i = 0; i < iterations; i++)
	{
		k++;
		z = complex_square(z) + c; 
		zlength = sqrt(z.x * z.x + z.y * z.y);
		if (zlength > 4.0)
		{
			break; 
		}

	}

	float h = abs(mod(time * 15.0 - float(k), 50.0) / 660.0);
	vec3 rgb = hsv(h, 1.0, 1.0);

	gl_FragColor = vec4(rgb, 1.0);
}
