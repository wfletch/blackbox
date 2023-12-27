mkdir -p $PROJECT_ROOT/compiled_shaders
$VK_GLSLC $PROJECT_ROOT/shaders/simple_shader.vert -o $PROJECT_ROOT/compiled_shaders/simple_shader.vert.spv
$VK_GLSLC $PROJECT_ROOT/shaders/simple_shader.frag -o $PROJECT_ROOT/compiled_shaders/simple_shader.frag.spv

echo "Shader's Compiled"