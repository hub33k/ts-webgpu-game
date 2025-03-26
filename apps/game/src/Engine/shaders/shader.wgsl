struct VertexInput {
    @builtin(vertex_index) vertexIndex: u32,
}

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) color: vec4f,
}

@vertex
fn vertex(
    in: VertexInput,
) -> VertexOutput {
    let pos = array(
        vec2(-0.5, -0.5),
        vec2( 0.5, -0.5),
        vec2( 0.5,  0.5),
    );

    var output: VertexOutput;
    output.position = vec4(pos[in.vertexIndex], 0.0, 1.0);
    output.color = vec4(1.0, 1.0, 1.0, 1.0);
    return output;
}

@fragment
fn fragment(input: VertexOutput) -> @location(0) vec4f {
    return input.color;
}
