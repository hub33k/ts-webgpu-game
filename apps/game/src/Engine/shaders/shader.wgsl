struct VertexInput {
    @builtin(vertex_index) vertexIndex: u32,
    @location(0) position: vec2f,
}

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) color: vec4f,
    @location(1) @interpolate(flat) vertexIndex: u32,
}

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

    output.color = vec4(1.0, 0.0, 1.0, 1.0);
    output.vertexIndex = in.vertexIndex;

    return output;
}

@fragment
fn fragment(in: VertexOutput) -> @location(0) vec4f {
    let vertexIndex = f32(in.vertexIndex);

    let r = sin(vertexIndex * 32.0);
    let g = sin(vertexIndex * 2.0 + 1.0);
    let b = sin(vertexIndex * 2.0 + 2.0);

    let color = vec4(r, g, b, 1.0);

    return color;
}
