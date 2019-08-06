#pragma once

static const char *vertex_shader_source = 
"#version 330 core\n"
"layout (location = 0) in vec2 vertex;\n"
"layout (location = 1) in vec2 offset;\n"
"uniform mat4 projectionMatrix;\n"
"uniform mat4 cameraMatrix;\n"
"void main() {\n"
"   gl_Position = projectionMatrix * cameraMatrix * vec4(vertex + offset, 0.0, 1.0);\n"
"}\0";

static const char *fragment_shader_source = 
"#version 330 core\n"
"out vec4 fragColor;\n"
"uniform vec3 color;\n"
"void main() {\n"
"	fragColor = vec4(color, 1.0f);\n"
"};\0";

//static const char *fragment_shader_source =
//"#version 330 core\n"
//"out vec4 fragColor;\n"
//"uniform vec3 color[120000];\n"
//"void main() {\n"
//"	fragColor = vec4(color[gl_InstanceID], 1.0f);\n"
//"};\0";