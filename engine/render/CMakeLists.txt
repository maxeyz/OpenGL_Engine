#--------------------------------------------------------------------------
# render
#--------------------------------------------------------------------------

SET(files_render_display
	window.h
	window.cc
	grid.h
	grid.cc
	MeshResource.h
	MeshResource.cpp
	Camera.h
	Camera.cpp
	TextureResource.h
	TextureResource.cpp
	ShaderResource.h
	ShaderResource.cpp
	GraphicNode.h
	GraphicNode.cpp
	InputManager.h
	LightingResource.h
	LightingResource.cpp
	Materials.h
	Materials.cpp
	GLTF.h
	GLTF.cpp
	)
SOURCE_GROUP("display" FILES ${files_render_display})


SET(files_render
	${files_render_display}
	${files_render_pch})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(render STATIC ${files_render} ${files_pch})
TARGET_PCH(render ../)	
ADD_DEPENDENCIES(render glew glfw)
TARGET_LINK_LIBRARIES(render PUBLIC engine exts glew glfw imgui ${OPENGL_LIBS})