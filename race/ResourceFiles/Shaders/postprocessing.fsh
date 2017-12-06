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

vec3 blurSampleGaussian(float dofblur)
{
	dofblur *= 0.25;

	vec4 col = vec4(0.0);
	
	//row 1
	col += texture(fBuffer, vec2(uv.x - 1.0 * dofblur, uv.y + 3.0 * dofblur)) * 0.001;
	col += texture(fBuffer, vec2(uv.x, uv.y + 3.0 * dofblur)) * 0.001;
	col += texture(fBuffer, vec2(uv.x + 1.0 * dofblur, uv.y + 3.0 * dofblur)) * 0.001;
	
	//row 2
	col += texture(fBuffer, vec2(uv.x - 2.0 * dofblur, uv.y + 2.0 * dofblur)) * 0.002;
	col += texture(fBuffer, vec2(uv.x - 1.0 * dofblur, uv.y + 2.0 * dofblur)) * 0.012;
	col += texture(fBuffer, vec2(uv.x, uv.y + 2.0 * dofblur)) * 0.020;
	col += texture(fBuffer, vec2(uv.x + 1.0 * dofblur, uv.y + 2.0 * dofblur)) * 0.012;
	col += texture(fBuffer, vec2(uv.x + 2.0 * dofblur, uv.y + 2.0 * dofblur)) * 0.002;
	
	//row 3
	col += texture(fBuffer, vec2(uv.x - 3.0 * dofblur, uv.y + 1.0 * dofblur)) * 0.001;
	col += texture(fBuffer, vec2(uv.x - 2.0 * dofblur, uv.y + 1.0 * dofblur)) * 0.012;
	col += texture(fBuffer, vec2(uv.x - 1.0 * dofblur, uv.y + 1.0 * dofblur)) * 0.068;
	col += texture(fBuffer, vec2(uv.x, uv.y + 1.0 * dofblur)) * 0.109;
	col += texture(fBuffer, vec2(uv.x + 1.0 * dofblur, uv.y + 1.0 * dofblur)) * 0.068;
	col += texture(fBuffer, vec2(uv.x + 2.0 * dofblur, uv.y + 1.0 * dofblur)) * 0.012;
	col += texture(fBuffer, vec2(uv.x + 3.0 * dofblur, uv.y + 1.0 * dofblur)) * 0.001;
	
	//row 4
	col += texture(fBuffer, vec2(uv.x - 3.0 * dofblur, uv.y)) * 0.001;
	col += texture(fBuffer, vec2(uv.x - 2.0 * dofblur, uv.y)) * 0.020;
	col += texture(fBuffer, vec2(uv.x - 1.0 * dofblur, uv.y)) * 0.109;
	col += texture(fBuffer, vec2(uv.x, uv.y)) * 0.172;
	col += texture(fBuffer, vec2(uv.x + 1.0 * dofblur, uv.y)) * 0.109;
	col += texture(fBuffer, vec2(uv.x + 2.0 * dofblur, uv.y)) * 0.020;
	col += texture(fBuffer, vec2(uv.x + 3.0 * dofblur, uv.y)) * 0.001;
	
	//row 5
	col += texture(fBuffer, vec2(uv.x - 3.0 * dofblur, uv.y - 1.0 * dofblur)) * 0.001;
	col += texture(fBuffer, vec2(uv.x - 2.0 * dofblur, uv.y - 1.0 * dofblur)) * 0.012;
	col += texture(fBuffer, vec2(uv.x - 1.0 * dofblur, uv.y - 1.0 * dofblur)) * 0.068;
	col += texture(fBuffer, vec2(uv.x, uv.y - 1.0 * dofblur)) * 0.109;
	col += texture(fBuffer, vec2(uv.x + 1.0 * dofblur, uv.y - 1.0 * dofblur)) * 0.068;
	col += texture(fBuffer, vec2(uv.x + 2.0 * dofblur, uv.y - 1.0 * dofblur)) * 0.012;
	col += texture(fBuffer, vec2(uv.x + 3.0 * dofblur, uv.y - 1.0 * dofblur)) * 0.001;
	
	//row 6
	col += texture(fBuffer, vec2(uv.x - 2.0 * dofblur, uv.y - 2.0 * dofblur)) * 0.002;
	col += texture(fBuffer, vec2(uv.x - 1.0 * dofblur, uv.y - 2.0 * dofblur)) * 0.012;
	col += texture(fBuffer, vec2(uv.x, uv.y - 2.0 * dofblur)) * 0.020;
	col += texture(fBuffer, vec2(uv.x + 1.0 * dofblur, uv.y - 2.0 * dofblur)) * 0.012;
	col += texture(fBuffer, vec2(uv.x + 2.0 * dofblur, uv.y - 2.0 * dofblur)) * 0.002;
	
	//row 7
	col += texture(fBuffer, vec2(uv.x - 1.0 * dofblur, uv.y - 3.0 * dofblur)) * 0.001;
	col += texture(fBuffer, vec2(uv.x, uv.y - 3.0 * dofblur)) * 0.001;
	col += texture(fBuffer, vec2(uv.x + 1.0 * dofblur, uv.y - 3.0 * dofblur)) * 0.001;
	
	return col.rgb;
}

vec3 blurSample2(float dofblur)
{
	float hstep = 0.1;
	float vstep = 0.1;

	vec4 col = vec4(0.0);

	col += texture(fBuffer, vec2(uv.x - 3.0*dofblur*hstep, uv.y)) * 0.0940540541;
	col += texture(fBuffer, vec2(uv.x - 2.0*dofblur*hstep, uv.y - 2.0*dofblur*vstep)) * 0.1216216216;
	col += texture(fBuffer, vec2(uv.x - 1.0*dofblur*hstep, uv.y - 1.0*dofblur*vstep)) * 0.2945945946;
	
	col += texture(fBuffer, vec2(uv.x + 3.0*dofblur*hstep, uv.y)) * 0.0940540541;
	col += texture(fBuffer, vec2(uv.x + 2.0*dofblur*hstep, uv.y - 2.0*dofblur*vstep)) * 0.1216216216;
	col += texture(fBuffer, vec2(uv.x + 1.0*dofblur*hstep, uv.y - 1.0*dofblur*vstep)) * 0.2945945946;

	//col += texture(fBuffer, vec2(uv.x, uv.y)) * 0.2270270270;
	col += texture(fBuffer, vec2(uv.x, uv.y)) * (1.0 - dofblur);
	
	col += texture(fBuffer, vec2(uv.x + 1.0*dofblur*hstep, uv.y + 1.0*dofblur*vstep)) * 0.2945945946;
	col += texture(fBuffer, vec2(uv.x + 2.0*dofblur*hstep, uv.y + 2.0*dofblur*vstep)) * 0.1216216216;
	col += texture(fBuffer, vec2(uv.x, uv.y + 3.0*dofblur*vstep)) * 0.0940540541;

	col += texture(fBuffer, vec2(uv.x - 1.0*dofblur*hstep, uv.y + 1.0*dofblur*vstep)) * 0.2945945946;
	col += texture(fBuffer, vec2(uv.x - 2.0*dofblur*hstep, uv.y + 2.0*dofblur*vstep)) * 0.1216216216;
	col += texture(fBuffer, vec2(uv.x, uv.y - 3.0*dofblur*vstep)) * 0.0940540541;	

	return col.rgb;
}

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
	

	//depth of field
	float cDepth = texture(dBuffer, vec2(0.5, 0.5)).r;
	//float depthDiff = clamp(abs(depth - cDepth), 0, 4.0);
	float depthDiff = clamp(abs(depth - cDepth), 0, 1.0);
	float biasedDepth = depthDiff * 10.0;
	//float biasedDepth = pow(depthDiff, 2.0) * 10.0;
	//vec3 blurColor = blurSample(biasedDepth * 0.1 * dofFactor) / 41.0;
	vec3 blurColor = blurSampleGaussian(biasedDepth * 0.1 * dofFactor);
	vec3 dofBlurFragColor = mix(fColor, blurColor, dofAmount);
	
	//fog
	float fogDist = depth * 10.0;
	float fogValue = clamp( exp(-fogFactor*fogDist), 0.0, 1.0);

	//blend DoF-blurred scene with smearbuffer for motion blur
	gl_FragColor.rgb = mix(dofBlurFragColor, sColor, blurAmount);
	
	//blend fog
	gl_FragColor.rgb = mix(gl_FragColor.rgb, fogColor, fogValue * fogAmount);

}

