CC = g++
FLAGS = -std=c++23 -g -ggdb -Wall -Wextra -Wno-missing-field-initializers
SRC_DIR = src

PLATFORM = linux_amd64
OSFLAG = $(shell uname)
ifeq ($(OSFLAG), Darwin)
	PLATFORM = macos
else
	ifeq ($(OSFLAG), Windows)
		$(error NO WINDOWS ALLOWED)
	endif
endif

$(info PLAFTORM: $(PLATFORM))
RAY_DIR = vendor/raylib-5.5_$(PLATFORM)
LIB_DIRS = -L$(RAY_DIR)/lib
LIBS = -l:libraylib.a
INCLUDES = -I$(RAY_DIR)/include/ -Isrc

ifeq ($(PLATFORM), macos)
	LIB_DIRS := -Wl,-rpath,$(RAY_DIR)/lib/ $(LIB_DIRS)
	LIBS = -lraylib
endif

EXECUTABLE = adiv

OBJECTS = \
build/Collider.o \
build/ComponentManager.o \
build/EntityManager.o \
build/SystemManager.o \
build/Registry.o \
build/CameraSystem.o \
build/CollisionSystem.o \
build/PhysicsSystem.o \
build/PlayerSystem.o \
build/RenderCollidersSystem.o \
build/RenderSystem.o \
build/AnimatorSystem.o \
build/SpriteAnimationSystem.o \
build/Window.o \
build/Game.o \
build/Level.o \
build/main.o \
build/MapLoader.o \
build/Helper.o 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) $(INCLUDES) $(LIB_DIRS) $(LIBS) -o $@

build/Collider.o: src/ecs/components/Collider.cpp src/ecs/components/Collider.h \
src/ecs/ECS.h \
src/ecs/Components.h src/ecs/components/Player.h \
src/ecs/components/RigidBody.h src/ecs/components/Sprite.h \
src/ecs/components/Transform.h src/ecs/Registry.h \
src/ecs/managers/SystemManager.h src/ecs/managers/EntityManager.h \
src/ecs/managers/ComponentManager.h src/ecs/managers/ComponentArray.h \
src/util/Helper.h src/ecs/components/Camera.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/components/Collider.cpp -o $@

build/ComponentManager.o: src/ecs/managers/ComponentManager.cpp \
src/ecs/managers/ComponentManager.h src/ecs/ECS.h \
src/ecs/managers/ComponentArray.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/managers/ComponentManager.cpp -o $@

build/EntityManager.o: src/ecs/managers/EntityManager.cpp \
src/ecs/managers/EntityManager.h src/ecs/ECS.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/managers/EntityManager.cpp -o $@

build/SystemManager.o: src/ecs/managers/SystemManager.cpp \
src/ecs/managers/SystemManager.h src/ecs/ECS.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/managers/SystemManager.cpp -o $@

build/Registry.o: src/ecs/Registry.cpp src/ecs/Registry.h src/ecs/ECS.h \
src/ecs/managers/SystemManager.h src/ecs/managers/EntityManager.h \
src/ecs/managers/ComponentManager.h src/ecs/managers/ComponentArray.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/Registry.cpp -o $@

build/CollisionSystem.o: src/ecs/systems/CollisionSystem.cpp \
src/ecs/systems/CollisionSystem.h src/ecs/ECS.h \
src/ecs/components/Collider.h \
src/ecs/Registry.h \
src/ecs/managers/SystemManager.h src/ecs/managers/EntityManager.h \
src/ecs/managers/ComponentManager.h src/ecs/managers/ComponentArray.h \
src/ecs/components/RigidBody.h src/ecs/components/Transform.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/systems/CollisionSystem.cpp -o $@

build/PhysicsSystem.o: src/ecs/systems/PhysicsSystem.cpp \
src/ecs/systems/PhysicsSystem.h src/ecs/ECS.h \
src/ecs/components/RigidBody.h \
src/ecs/components/Transform.h src/ecs/Registry.h \
src/ecs/managers/SystemManager.h src/ecs/managers/EntityManager.h \
src/ecs/managers/ComponentManager.h src/ecs/managers/ComponentArray.h \
src/util/Helper.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/systems/PhysicsSystem.cpp -o $@

build/PlayerSystem.o: src/ecs/systems/PlayerSystem.cpp \
src/ecs/systems/PlayerSystem.h src/ecs/ECS.h \
src/ecs/Registry.h \
src/ecs/managers/SystemManager.h src/ecs/managers/EntityManager.h \
src/ecs/managers/ComponentManager.h src/ecs/managers/ComponentArray.h \
src/ecs/components/RigidBody.h \
src/ecs/components/Transform.h \
src/util/Helper.h src/Animation.h \
src/ecs/components/Animator.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/systems/PlayerSystem.cpp -o $@

build/RenderCollidersSystem.o: src/ecs/systems/RenderCollidersSystem.cpp \
src/ecs/systems/RenderCollidersSystem.h src/ecs/ECS.h \
src/ecs/components/Collider.h \
src/ecs/Registry.h \
src/ecs/managers/SystemManager.h src/ecs/managers/EntityManager.h \
src/ecs/managers/ComponentManager.h src/ecs/managers/ComponentArray.h \
src/util/Helper.h src/ecs/Components.h src/ecs/components/Player.h \
src/ecs/components/RigidBody.h src/ecs/components/Sprite.h \
src/ecs/components/Transform.h src/ecs/components/Camera.h 
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/systems/RenderCollidersSystem.cpp -o $@

build/RenderSystem.o: src/ecs/systems/RenderSystem.cpp \
src/ecs/systems/RenderSystem.h src/ecs/ECS.h src/ecs/components/Sprite.h \
src/ecs/components/Transform.h src/ecs/components/RigidBody.h \
src/util/Helper.h src/ecs/Components.h src/ecs/components/Player.h \
src/ecs/components/Collider.h src/ecs/Registry.h \
src/ecs/managers/SystemManager.h src/ecs/managers/EntityManager.h \
src/ecs/managers/ComponentManager.h src/ecs/managers/ComponentArray.h \
src/ecs/components/Camera.h 
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/systems/RenderSystem.cpp -o $@

build/CameraSystem.o: src/ecs/systems/CameraSystem.cpp \
src/ecs/systems/CameraSystem.h src/ecs/ECS.h src/ecs/components/Sprite.h \
src/ecs/components/Transform.h src/ecs/components/RigidBody.h \
src/ecs/Components.h src/ecs/components/Player.h \
src/ecs/components/Collider.h src/ecs/Registry.h \
src/ecs/managers/SystemManager.h src/ecs/managers/EntityManager.h \
src/ecs/managers/ComponentManager.h src/ecs/managers/ComponentArray.h \
src/ecs/components/Camera.h src/util/Helper.h \
src/Window.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/systems/CameraSystem.cpp -o $@

build/AnimatorSystem.o: src/ecs/systems/AnimatorSystem.cpp \
src/ecs/systems/AnimatorSystem.h src/ecs/ECS.h src/ecs/components/Sprite.h \
src/ecs/components/Transform.h src/ecs/components/RigidBody.h \
src/ecs/Components.h src/ecs/components/Player.h \
src/ecs/components/Collider.h src/ecs/Registry.h \
src/ecs/managers/SystemManager.h src/ecs/managers/EntityManager.h \
src/ecs/managers/ComponentManager.h src/ecs/managers/ComponentArray.h \
src/ecs/components/Animator.h 
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/systems/AnimatorSystem.cpp -o $@

build/SpriteAnimationSystem.o: src/ecs/systems/SpriteAnimationSystem.cpp \
src/ecs/systems/SpriteAnimationSystem.h src/ecs/ECS.h src/ecs/components/Sprite.h \
src/ecs/Components.h src/ecs/Registry.h \
src/ecs/managers/SystemManager.h src/ecs/managers/EntityManager.h \
src/ecs/managers/ComponentManager.h src/ecs/managers/ComponentArray.h \
src/ecs/components/Animator.h src/ecs/components/SpriteAnimation.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/ecs/systems/SpriteAnimationSystem.cpp -o $@

build/Game.o: src/Game.cpp src/Game.h \
src/Window.h src/MapLoader.h \
src/ecs/ECS.h src/ecs/Registry.h src/ecs/managers/SystemManager.h \
src/ecs/managers/EntityManager.h src/ecs/managers/ComponentManager.h \
src/ecs/managers/ComponentArray.h src/ecs/Systems.h \
src/ecs/systems/CollisionSystem.h src/ecs/components/Collider.h \
src/ecs/systems/PhysicsSystem.h src/ecs/components/RigidBody.h \
src/ecs/components/Transform.h src/ecs/systems/PlayerSystem.h \
src/ecs/systems/RenderCollidersSystem.h src/ecs/systems/RenderSystem.h \
src/ecs/components/Sprite.h src/util/Helper.h src/ecs/Components.h \
src/ecs/components/Player.h src/util/Random.h \
src/Level.h src/ecs/components/Camera.h \
src/ecs/systems/CameraSystem.h src/ecs/systems/AnimatorSystem.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/Game.cpp -o $@

build/main.o: src/main.cpp src/Game.h \
src/Window.h \
src/ecs/ECS.h src/ecs/Registry.h src/ecs/managers/SystemManager.h \
src/ecs/managers/EntityManager.h src/ecs/managers/ComponentManager.h \
src/ecs/managers/ComponentArray.h src/ecs/Systems.h \
src/ecs/systems/CollisionSystem.h src/ecs/components/Collider.h \
src/ecs/systems/PhysicsSystem.h src/ecs/components/RigidBody.h \
src/ecs/components/Transform.h src/ecs/systems/PlayerSystem.h \
src/ecs/systems/RenderCollidersSystem.h src/ecs/systems/RenderSystem.h \
src/ecs/components/Sprite.h src/util/Helper.h src/ecs/Components.h \
src/ecs/components/Player.h src/ecs/components/Camera.h \
src/ecs/systems/CameraSystem.h src/ecs/systems/AnimatorSystem.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/main.cpp -o $@

build/Helper.o: src/util/Helper.cpp src/util/Helper.h \
src/ecs/Components.h \
src/ecs/components/Player.h src/ecs/components/Collider.h \
src/ecs/ECS.h \
src/ecs/components/RigidBody.h src/ecs/components/Sprite.h \
src/ecs/components/Transform.h src/ecs/components/Camera.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/util/Helper.cpp -o $@

build/Window.o: src/Window.cpp src/Window.h 
	$(CC) $(FLAGS) $(INCLUDES) -c src/Window.cpp -o $@

build/MapLoader.o: src/MapLoader.cpp src/MapLoader.h 
	$(CC) $(FLAGS) $(INCLUDES) -c src/MapLoader.cpp -o $@

build/Level.o: src/Level.cpp src/Level.h \
src/MapLoader.h src/ecs/ECS.h \
src/Game.h src/ecs/components/Sprite.h \
src/ecs/Registry.h src/ecs/components/Camera.h \
src/Window.h src/ecs/components/SpriteAnimation.h
	$(CC) $(FLAGS) $(INCLUDES) -c src/Level.cpp -o $@

.PHONY : clean
clean:
	rm -f $(EXECUTABLE) build/*.o
