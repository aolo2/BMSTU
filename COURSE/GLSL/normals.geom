#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out GS_OUT {
    vec3 color;
} gs_out;

void main() {
    gs_out.color = gs_in[0].color;

    vec3 normal = normalize(cross(gs_in[1].glPosition - gs_in[0].glPosition, gs_in[1].glPosition - gs_in[2].glPosition));
    
    glPosition = glPosition[0];
    EmitVertex();
    glPosition = glPosition[1];
    EmitVertex();
    glPosition = glPosition[2];
    EmitVertex();

    EndPrimitive();

    glPosition = glPosition[0];
    EmitVertex();
    glPosition = glPosition[0] + normal;
    EmitVertex();
    glPosition = glPosition[1];
    EmitVertex();

    EndPrimitive();
}