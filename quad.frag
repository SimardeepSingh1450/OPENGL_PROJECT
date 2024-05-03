#version 430 core

//Input
layout (location=0) in vec2 textureCoordsIn;

//(Output) The only difference between vertex shader and fragment shader is that here we supply output color
layout (location=0) out vec4 fragColor;

//Bindings
layout (location=0) uniform sampler2D textureAtlas;

void main(){
	vec4 textureColor = texelFetch(textureAtlas,ivec2(textureCoordsIn),0);

	//Supplying color white as output color - White Quad
	fragColor = textureColor;
}