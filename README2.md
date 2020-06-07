
- [C++11, Good way of using c++](#C++11)


<a name="C++11"></a>
## C++ 11 
Resource : CppCon 2016: Jason Jurecka "Game Engine usig STD c++11"        
https://www.youtube.com/watch?v=8AjRD6mU96s      
### Advantage for Game Engine.
* Compile time resource versioning using constexpr
* Compile time type validation using static_assert
* Compile time serialize/deserialize function lists

```
struct Vec3
{
   float x = 0.0f;
   float y = 0.0f;
   float z = 0.0f;
};
struct SpawnPoint
{
    Vec3 m_Position;
    Vec3 m_Direction;
}

```

## Mass Concurrency
Most devices now have multiple cores and thread support.
* Benefits 
- Exploit multiple processor machines.
- Exploit cloud processing(dynamic scaling)
- Tasks can be small and simple (focused optimizations)
- Serialized sequences can be distributed.
- Highly modular for version-ing of tasks/ systems.    
- Efficiency of doing multiple things at once out weighs
serialized process costs.      

* Concerns:
- Tasks need to be as independent as possible.     
- Data manipulation needs to be thread safe(dead-locks or live-locks)      
- Need to keep in mind when data synchronization happens.
- Need to account  for things executing in non-determined order.
- Task queue speed (lock vs lock-free)    
- Thread Context switching.      














Reference:
https://dzone.com/articles/how-important-is-data-in-game-development     
- Keys to develop successful games.
leverage data to constrantly improve the game and personalize the experience of each player.
 Capturing game play data + tracking players in real-time, and the ability to bring
 data-driven insights into the real-time personalization of the game.   
 
 - Personalize the UX. C

## Architecture


## Database 


#### VoltDB? 
VoltDB provides an in-memory, ranslytical database that's able to ingest millions of
transactions per second.      
Real-time Analytics & Real time decisions.
### 

