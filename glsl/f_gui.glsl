uniform sampler2D sMainTex;

in vec2 fragUV1;

out vec4 fragColor;

void main() {
    vec2 uv = vec2(uUV * vec3(fragUV1, 1.0));
    vec4 mainTexSample = texture(sMainTex, uv);
    vec3 objectColor = uColor.rgb * mainTexSample.rgb;
    if (isFeatureEnabled(FEATURE_DISCARD) && length(uDiscardColor.rgb - objectColor) < 0.01) {
        discard;
    }
    fragColor = vec4(objectColor, uAlpha * mainTexSample.a);
}
