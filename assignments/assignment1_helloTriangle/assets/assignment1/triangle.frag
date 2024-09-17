#version 330 core
in vec3 pos;
in vec4 color;

uniform float time;

out vec4 FragColor;


void main() {
	float b1 = (pos.y + 0.5f) * 2;
	float a1 = (-(pos.x - 0.5) * 2) - b1 / 2f;
	float c1 = ((pos.x + 0.5f) * 2) - b1 / 2f;

	float a = a1 / (a1 + b1 + c1) * 1.3333f;
	float b = b1 / (a1 + b1 + c1) * 1.3333f;
	float c = c1 / (a1 + b1 + c1) * 1.3333f;

	vec4 rgb;

	int timeStamp = 3; //must be multiple of 3, I tried to make this an int uniform but it kept breaking?

	int stage = int(time) % timeStamp;
	float fl = ((stage % (timeStamp / 3)) + (time - int(time))) / (timeStamp / 3);
	if(stage < timeStamp / 3) {
		rgb.r = mix(a, b, fl);
		rgb.g = mix(b, c, fl);
		rgb.b = mix(c, a, fl);
	} else if(stage >= timeStamp * 2 / 3) {
		rgb.r = mix(c, a, fl);
		rgb.g = mix(a, b, fl);
		rgb.b = mix(b, c, fl);
	} else {
		rgb.r = mix(b, c, fl);
		rgb.g = mix(c, a, fl);
		rgb.b = mix(a, b, fl);
	}

	FragColor = vec4(rgb.r, rgb.g, rgb.b, 1.0f);
}