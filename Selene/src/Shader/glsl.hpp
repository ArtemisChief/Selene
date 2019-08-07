#pragma once

static const char *vertex_shader_source = 
"#version 330 core\n"
"layout (location = 0) in vec2 vertex;\n"
"layout (location = 1) in vec2 offset;\n"
"uniform mat4 projectionMatrix;\n"
"uniform mat4 cameraMatrix;\n"
"out vec3 fColor;\n"
"void main() {\n"
"   gl_Position = projectionMatrix * cameraMatrix * vec4(vertex + offset, 0.0, 1.0);\n"
"   fColor = vec3(gl_InstanceID / 10800.0, gl_InstanceID / 10800.0, gl_InstanceID / 10800.0);\n"
"}\0";

static const char *fragment_shader_source =
"#version 330 core\n"
"in vec3 fColor;\n"
"out vec4 fragColor;\n"
"void main() {\n"
"	fragColor = vec4(fColor, 1.0f);\n"
"};\0";