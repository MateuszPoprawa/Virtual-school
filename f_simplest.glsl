#version 330

uniform sampler2D textureMap; //zmienna reprezentująca jednostkę teksturującą

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 ic; 
in vec4 n;
in vec4 l1;
in vec4 v;
in vec2 iTexCoord; //Zmienna do interpolacji współrzędnych teksturowania
in vec4 l2;
in vec4 l3;


void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 ml1 = normalize(l1);
	vec4 ml2 = normalize(l2);
	vec4 ml3 = normalize(l3);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	//Wektor odbity
	vec4 mr1 = reflect(-ml1, mn);
	vec4 mr2 = reflect(-ml2, mn);
	vec4 mr3 = reflect(-ml3, mn);

	//Parametry powierzchni
	vec4 kd = texture(textureMap, iTexCoord);
	vec4 ks = vec4(1, 1, 1, 1);

	//Obliczenie modelu oświetlenia
	float nl1 = clamp(dot(mn, ml1), 0, 1);
	float nl2 = clamp(dot(mn, ml2), 0, 1);
	float nl3 = clamp(dot(mn, ml3), 0, 1);
	float rv1 = pow(clamp(dot(mr1, mv), 0, 1), 50);
	float rv2 = pow(clamp(dot(mr2, mv), 0, 1), 50);
	float rv3 = pow(clamp(dot(mr3, mv), 0, 1), 50);
	//pixelColor= vec4(kd.rgb * nl, kd.a) + vec4(ks.rgb*rv, 0);
	
	vec4 texColor = texture(textureMap, iTexCoord); //Pobranie koloru

	pixelColor = vec4(texColor.rgb*nl1, texColor.a) + vec4(ks.rgb*rv1, 0)+
	vec4(texColor.rgb*nl2, texColor.a)+vec4(ks.rgb*rv2, 0)+
	vec4(texColor.rgb*nl3, texColor.a)+vec4(ks.rgb*rv3, 0);
}
