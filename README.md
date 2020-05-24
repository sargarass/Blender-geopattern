# Blender-geopattern
Studing project: main goal of this project was to learn how real production ready gpu raytracers formulate images and to add new unsupported feature to such raytracer's pipeline. That feature should works on gpu during ray-mesh intercection stage.
*If you are looking for production ready solution take a look at [Fstorm](https://fstormrender.ru) or [Centileo](www.centileo.com) (in future). Current implementation has it's own limits and bugs/artifacts that are not allowing to use it in any kind of production*

[Geopattern](https://fstormrender.ru/manual/geopattern/) can be described as facility to use mesh in a way as textures can be used, allowing us to create new objects by spreading (mapping) one object onto the surface of another.
Without a significant increase in memory consumption, same as instancing does. You still have same number of meshes in a scene, it just changes the way each ray intersect with it.


# Examples (all images are clickable):
*Note: in all examples texture cordinates for mapping were automatically generated, cropboxes created by hands. More about those magical words [here](https://fstormrender.ru/manual/geopattern/) (we use same terminology)*

Starting from simple one to explain what it does: by having piece of a chain and a sphere it can create sphere full covered by chain:
| *Patch* of a chain  |  Simple sphere |
|-------------------------|------------------------------|
 <img src="images/piece_of_chain.png" width="425"/> | <img src="images/sphere.png" width="425"/> 

|Result| 
|:--:| 
| <img src="images/chained_sphere.png" width="425"/> |

Same piece of chain (436K triangles) was spread over t-short model (4377 triangles). Armor created using with that feature. With this feature it used ~25MB of memory while without would would have to use ~53GB of VRAM 
(assuming that each triangle was covered with fully detailed chain)
|Armor created using that feature. Storing mesh without would cost 53GB of VRAM | 
|:--:| 
| <img src="images/armor.png" width="1000"/> |

|Close lookup | 
|:--:| 
| <img src="images/armor_closelook.png" width="1000"/> |

| Grass over mountain (landscape 67K triangles, grass 50K) ~3.9MB VRAM to storing mesh. 9.7GB without feature | 
|:--:| 
| <img src="images/grass.png" width="1000"/> |
| <img src="images/GrassField_closeup.png" width="1000"/> |


| Fibers example | 
|:--:| 
| <img src="images/girl.png" width="1000"/> |
| <img src="images/fibers.png" width="1000"/> |

| Utah teapot: torus and fibers were used as mapping geometry | 
|:--:| 
| <img src="images/utah.png" width="1000"/> |
| <img src="images/utah1.png" width="1000"/> |
| <img src="images/utah2.png" width="1000"/> |
