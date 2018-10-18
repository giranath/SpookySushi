# ECS

The Sushi engine uses ECS to describe it's game objects.

The framework must be the must decoupled possible.

* Entities should only be used to group components together.
* Plugins can add new components and systems

## Entity
An entity is only a key.

### Entity Registry
A registry is necessary to ensure entity uniqueness in the framework. It is responsible to create entity and destroy them.

## Component
A component only contains data. It should be a POD type making it possible to copy it's raw memory across multiple threads and systems.
No hierarchy is necessary.

### Component Storage
Components can be stored in multiple types of storages. A component storage must satisfy a particular interface.

#### Dictionary storage
Components are stored in a binary tree. This kind of storage should be the default choice as it should be efficient with
standard components that are used in multiple entities but not all.

#### Contiguous storage
Components are stored in a vector. This kind of storage is interesting when a particular kind of components is used by all 
entites. (For example, a transform component)

#### Indirect contiguous storage
Components are stored in vector adressed by another vector. This kind of storage is interesting for components that are common
for a majority of entities. It is similar to the contiguous storage but also support holes.

## System
