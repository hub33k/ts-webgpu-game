struct VertexInput {
    // @builtin(vertex_index) vertexIndex: u32,
    @location(0) position: vec4f,
}

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) color: vec4f,
}

@vertex
fn vertex(in: VertexInput) -> VertexOutput {
    var output: VertexOutput;
    const scale = 0.5;
    output.position = vec4f(in.position.xy / scale, in.position.z, in.position.w);
    output.color = vec4(1.0, 0.0, 1.0, 1.0);
    return output;
}

@fragment
fn fragment(in: VertexOutput) -> @location(0) vec4f {
    return in.color;
}
