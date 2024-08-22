#version 330 core

in vec2 texCoord;
in vec3 normal;

uniform sampler2D ourTexture;
//uniform float lighting;
uniform bool useGrayscale;
uniform float timer; // 添加時間變量

out vec4 FragColor;

void main()
{
	// TODO: Implement Grayscale Effect
	//   1. Retrieve the color from the texture at texCoord.
	//   2. If useGrayscale is true,
	//        a. Calculate the grayscale value using the dot product with the luminance weights(0.299, 0.587, 0.114).
	//        b. Set FragColor to a grayscale version of the color.
	//   Note: Ensure FragColor is appropriately set for both grayscale and color cases.
	
	// Retrieve the color from the texture at texCoord.
	vec4 color = texture2D(ourTexture, texCoord);

	if(useGrayscale)
	{
		// Calculate the grayscale value and set FragColor to the grayscale version
		float grayscale = dot(color.rgb, vec3(0.299, 0.587, 0.114));
		FragColor = vec4(grayscale, grayscale, grayscale, color.a);
	}
	else
    {
        // Compute the color variation based on time
        vec3 timeColor = vec3(sin(timer)+1.0, cos(timer), 1.0);
        
        // Apply the time-based color variation
        FragColor = vec4(color.rgb * timeColor, color.a);
    }
}