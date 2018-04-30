This is an OpenGL render engine which serves as a platform for users to create 3D scenes with ease. They are provided with data types such as Models, Lights, Cameras, Materials and Textures and SkyBoxes. They are given complete control over material properties such as diffuse, specular and ambient lighting, normal displacement and environmental reflection. The scene also allows for directional light shadows maps. All 3D scene objects (Light, Model, Camera, Skybox) are mutable through a transformable interface and can be added to a scene graph to allow for object parenting. All management of memory, buffers, and window resources are handled by the engine. This engine also uses a frame buffer to allow for post processing of the rendered 2D image.

To demo, download or clone the repo, open the solution, build and run. The files SampleScene.h and SampleScene.cpp contain the objects unique to the demo.