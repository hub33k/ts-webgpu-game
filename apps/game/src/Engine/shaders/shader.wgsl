struct VertexInput {
    @builtin(vertex_index) vertexIndex: u32,
    @location(0) position: vec2f,
    @location(1) texCoords: vec2f,
}

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) color: vec4f,
    @location(1) @interpolate(flat) vertexIndex: u32,
    @location(2) texCoords: vec2f,
}

@group(0) @binding(0)
var textureSampler: sampler;

@group(0) @binding(1)
var texture: texture_2d<f32>;

@vertex
fn vertex(in: VertexInput) -> VertexOutput {
    var output: VertexOutput;

    let useRatio = true;
    var ratio: f32 = 1.0;
    if (useRatio) {
        ratio = 800.0 / 600.0;
    }

    const scale = 1.0;
    output.position = vec4f(
        in.position.x * scale,
        in.position.y * scale * ratio,
        1.0,
        1.0,
    );

    output.color = vec4(1.0, 1.0, 1.0, 1.0);
    output.vertexIndex = in.vertexIndex;
    output.texCoords = in.texCoords;

    return output;
}

@fragment
fn fragment(in: VertexOutput) -> @location(0) vec4f {
    var textureColor = textureSample(texture, textureSampler, in.texCoords);

    return in.color * textureColor;
}
