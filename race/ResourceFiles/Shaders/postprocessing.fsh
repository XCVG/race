#version 330 core
in vec2 uv;
uniform sampler2D fBuffer; //normal framebuffer
uniform sampler2D dBuffer; //depth buffer
uniform sampler2D sBuffer; //smear buffer
uniform float blurAmount;
uniform float dofAmount;
uniform float dofFactor;
uniform float fogAmount;
uniform float fogFactor;
uniform vec3 fogColor;

vec3 blurSample(float dofblur)
{
	vec4 col = vec4(0.0);

    col += texture(fBuffer, uv);
    col += texture(fBuffer, uv + (vec2( 0.0,0.4 )) * dofblur);
    col += texture(fBuffer, uv + (vec2( 0.15,0.37 )) * dofblur);
    col += texture(fBuffer, uv + (vec2( 0.29,0.29 )) * dofblur);
    col += texture(fBuffer, uv + (vec2( -0.37,0.15 )) * dofblur);    
    col += texture(fBuffer, uv + (vec2( 0.4,0.0 )) * dofblur);   
    col += texture(fBuffer, uv + (vec2( 0.37,-0.15 )) * dofblur);    
    col += texture(fBuffer, uv + (vec2( 0.29,-0.29 )) * dofblur);    
    col += texture(fBuffer, uv + (vec2( -0.15,-0.37 )) * dofblur);
    col += texture(fBuffer, uv + (vec2( 0.0,-0.4 )) * dofblur);  
    col += texture(fBuffer, uv + (vec2( -0.15,0.37 )) * dofblur);
    col += texture(fBuffer, uv + (vec2( -0.29,0.29 )) * dofblur);
    col += texture(fBuffer, uv + (vec2( 0.37,0.15 )) * dofblur); 
    col += texture(fBuffer, uv + (vec2( -0.4,0.0 )) * dofblur);  
    col += texture(fBuffer, uv + (vec2( -0.37,-0.15 )) * dofblur);   
    col += texture(fBuffer, uv + (vec2( -0.29,-0.29 )) * dofblur);   
    col += texture(fBuffer, uv + (vec2( 0.15,-0.37 )) * dofblur);
    
    col += texture(fBuffer, uv + (vec2( 0.15,0.37 )) * dofblur*0.9);
    col += texture(fBuffer, uv + (vec2( -0.37,0.15 )) * dofblur*0.9);        
    col += texture(fBuffer, uv + (vec2( 0.37,-0.15 )) * dofblur*0.9);        
    col += texture(fBuffer, uv + (vec2( -0.15,-0.37 )) * dofblur*0.9);
    col += texture(fBuffer, uv + (vec2( -0.15,0.37 )) * dofblur*0.9);
    col += texture(fBuffer, uv + (vec2( 0.37,0.15 )) * dofblur*0.9);     
    col += texture(fBuffer, uv + (vec2( -0.37,-0.15 )) * dofblur*0.9);   
    col += texture(fBuffer, uv + (vec2( 0.15,-0.37 )) * dofblur*0.9);    
    
    col += texture(fBuffer, uv + (vec2( 0.29,0.29 )) * dofblur*0.7);
    col += texture(fBuffer, uv + (vec2( 0.4,0.0 )) * dofblur*0.7);   
    col += texture(fBuffer, uv + (vec2( 0.29,-0.29 )) * dofblur*0.7);    
    col += texture(fBuffer, uv + (vec2( 0.0,-0.4 )) * dofblur*0.7);  
    col += texture(fBuffer, uv + (vec2( -0.29,0.29 )) * dofblur*0.7);
    col += texture(fBuffer, uv + (vec2( -0.4,0.0 )) * dofblur*0.7);  
    col += texture(fBuffer, uv + (vec2( -0.29,-0.29 )) * dofblur*0.7);   
    col += texture(fBuffer, uv + (vec2( 0.0,0.4 )) * dofblur*0.7);
             
    col += texture(fBuffer, uv + (vec2( 0.29,0.29 )) * dofblur*0.4);
    col += texture(fBuffer, uv + (vec2( 0.4,0.0 )) * dofblur*0.4);   
    col += texture(fBuffer, uv + (vec2( 0.29,-0.29 )) * dofblur*0.4);    
    col += texture(fBuffer, uv + (vec2( 0.0,-0.4 )) * dofblur*0.4);  
    col += texture(fBuffer, uv + (vec2( -0.29,0.29 )) * dofblur*0.4);
    col += texture(fBuffer, uv + (vec2( -0.4,0.0 )) * dofblur*0.4);  
    col += texture(fBuffer, uv + (vec2( -0.29,-0.29 )) * dofblur*0.4);   
    col += texture(fBuffer, uv + (vec2( 0.0,0.4 )) * dofblur*0.4);
	
	return col.rgb;
}

void main()
{
	vec3 fColor = texture(fBuffer, uv).rgb;
	vec3 sColor = texture(sBuffer, uv).rgb;
	float depth = texture(dBuffer, uv).r;
	
	//output is always opaque
	gl_FragColor.a = 1.0;
	
	//motion blur
	vec3 mBlurFragColor = mix(fColor, sColor, blurAmount);
	
	//depth of field
	float cDepth = texture(dBuffer, vec2(0.5, 0.5)).r;
	//float depthDiff = clamp(abs(depth - cDepth), 0, 4.0);
	float depthDiff = abs(depth - cDepth);
	float biasedDepth = depthDiff * 10.0;
	vec3 blurColor = blurSample(biasedDepth * 0.1 * dofFactor) / 41.0;
	vec3 dofBlurFragColor = mix(fColor, blurColor, dofAmount);
	
	//fog
	float fogDist = depth * 10.0;
	float fogValue = clamp( exp(-fogFactor*fogDist), 0.0, 1.0);

	//blend blur
	gl_FragColor.rgb = (mBlurFragColor + dofBlurFragColor) / 2.0;
	
	//blend fog
	gl_FragColor.rgb = mix(gl_FragColor.rgb, fogColor, fogValue * fogAmount);

}

