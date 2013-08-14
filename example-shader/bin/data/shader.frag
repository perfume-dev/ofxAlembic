#define PI (3.1415926536)
#define TWO_PI (6.2831853072)

#ifdef GL_ES
precision mediump float;
#endif

uniform float elapsedTime;
varying vec3 position, normal;
varying float randomOffset;

float time = elapsedTime;
uniform vec2 mouse;
uniform vec2 resolution;


const vec4 on = vec4(1.);
const vec4 g  = vec4(0,1,0,1);
const vec4 off = vec4(vec3(0.), 1.);

void main() {
	float stages = 7.;
	float stage = mod(elapsedTime * .6, stages);

	if(stage < 1.) {
		float angle = atan(position.y,position.x)/(2.*3.14159265359);
		angle -= floor(angle);
		float rad = length(position);
		
		float color = 0.0;
		for (int i = 0; i < 100; i++) {
			float angleFract = fract(angle*32.);
			float angleRnd = floor(angle*32.)+1.;
			float angleRnd1 = fract(angleRnd*fract(angleRnd*.7235)*45.1);
			float angleRnd2 = fract(angleRnd*fract(angleRnd*.82657)*13.724);
			float t = time+angleRnd1*10.;
			float radDist = sqrt(angleRnd2+float(i)+2.);
			
			float adist = radDist/rad*.1;
			float dist = (t*.1+adist);
			dist = abs(fract(dist)-.5);
			float xdist = dist*5./adist;
			float ydist = abs(angleFract-.5);
			
			color += max(0.,.5-(xdist*xdist+ydist*ydist)*(adist*adist*radDist*radDist)*20.)*randomOffset;
			
			angle = fract(angle+.61);
		}
		gl_FragColor = vec4(0,color,color,color)*.3;

	} else if(stage < 2.) {
		vec2 position = ( 20.0 * gl_FragCoord.xy / resolution.xy ) / 4.0;

		float color = 0.0;
		color += sin( position.x * cos( time / 20.0 ) * 80.0 ) + cos( position.y * cos( time / 15.0 ) * 10.0 );
		color += sin( position.y * sin( time / 30.0 ) * 40.0 ) + cos( position.x * sin( time / 25.0 ) * 40.0 );
		color += sin( position.x * sin( time / 10.0 ) * 10.0 ) + sin( position.y * sin( time / 35.0 ) * 80.0 );
		color *= sin( time / 10.0 ) * 0.5;
		
		vec4 tempColor = vec4( vec3( color, color * 0.5, sin( color + time / 10.0 ) * 0.75 ), 1.0 );
	
		gl_FragColor = tempColor;

    } else if(stage < 3.) {
        gl_FragColor = vec4(sin(mod(position.x + elapsedTime*2., PI)),
                        sin(mod(-randomOffset + elapsedTime*2., TWO_PI)),
                        0,
                        1);
	} else if(stage < 4.) {
		// crazy triangles, grid lines
		float speed = 30.;
		float scale = 20.0;	
		float cutoff = .9;
		vec3 cur = mod(position + speed * elapsedTime, scale) / scale;
		cur *= 1. - abs(normal);
		gl_FragColor = (max(max(cur.x, cur.y), cur.z) < cutoff) ? off : vec4(sin(mod(position.x + elapsedTime*2., PI)),
                        sin(mod(-randomOffset + elapsedTime*2., TWO_PI)),
                        sin(mod(elapsedTime,PI)),
                        1);;
	} else if(stage < 5.) {

	    float sum = 0.0;
		float size = resolution.x / 1000.0;
    	for (int i = 0; i < 20; ++i) {
        	vec2 position = resolution / 2.0;
			float t = (float(i) + elapsedTime) / 5.0;
			float c = float(i) * 4.0;
 	    	position.x += tan(8.0 * t + c) * resolution.x * 0.2;
        	position.y += sin(t) * resolution.y * 0.8;

        	sum += size / length(gl_FragCoord.xy - position);
    	}
    	gl_FragColor = vec4(sum * 0.5, sum, sum * 3.1, 1);
    } else if (stage < 6.) {
    	gl_FragColor = vec4(normal * 0.5 + 0.5, 1);
    } else if (stage < 7.) {
    	float t = mod(elapsedTime * 30., 20.) + 5.;
    	float d = mod(position.y * 1., t) / t;
    	float a = sin(d * TWO_PI) * 0.5 + 0.5;

    	if (a > 0.5) gl_FragColor = vec4(1, 0, 0, 0.5);
	    else discard;
    }

}
