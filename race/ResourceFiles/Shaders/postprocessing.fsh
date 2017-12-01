#version 330 core
in vec2 uv;
uniform sampler2D fBuffer; //normal framebuffer
uniform sampler2D dBuffer; //depth buffer
uniform sampler2D sBuffer; //smear buffer
uniform float blurAmount;
uniform float dofAmount;
uniform float fogAmount;

vec3 blurSample(float dofblur)
{
	vec4 col = vec4(0.0);

    //col += texture(fBuffer, uv);
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
	gl_FragColor.rgb = mix(fColor, sColor, blurAmount);
	
	//depth of field
	float cDepth = texture(dBuffer, vec2(0.5, 0.5)).r;
	float depthDiff = abs(depth - cDepth);
	float biasedDepth = depthDiff * 10.0;
	vec3 blurColor = blurSample(0.05) / 40.0;
	gl_FragColor.rgb = mix(fColor, blurColor, biasedDepth*dofAmount);
	
}

