/*
 * RCS:
 * $Log: objectPool.cxx,v $
 * Revision 1.2  2024/09/10 20:02:21  praveen
 * Used vector reserve() to avoid push_back() trigger for fresh memory re-allocations.
 *
 * Revision 1.1  2024/09/07 21:42:54  praveen
 * Initial revision
 *
 */
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

class SocketObj {
public:
    SocketObj() : id_(objCount++) {
        // Constructor logic (e.g., initializing a socket)
        std::cout << "SocketObj created.\n";
    }

    SocketObj(SocketObj &_other) : id_(_other.id_) {
        // Constructor logic (e.g., initializing a socket)
        std::cout << "SocketObj copy created.\n";
    }

    ~SocketObj() {
        // Destructor logic (e.g., cleaning up the socket)
        std::cout << "SocketObj destroyed.\n";
    }

    void use() {
        // Example method to simulate using the object
        std::cout << "SocketObj in use.\n";
    }
private:
    int id_;
    static int objCount;
};

int SocketObj::objCount = 0;

class SocketObjPool {
public:
    // Constructor that pre-instantiates a fixed number of objects
    SocketObjPool(std::size_t poolSize) {
	std::shared_ptr<SocketObj> obj_sPtr;
	pool_.reserve(poolSize);
	freePool_.reserve(poolSize);
        for (std::size_t i = 0; i < poolSize; ++i) {
	     obj_sPtr = std::make_shared<SocketObj>();
            //pool_.emplace_back(std::make_shared<SocketObj>());
	    pool_.push_back(obj_sPtr);
            freePool_.push_back(obj_sPtr);
        }
    }

    // Method to acquire an object from the pool
    std::shared_ptr<SocketObj> acquire() {
        if (freePool_.empty()) {
	  if (!garbageCollect()) {
	    throw std::runtime_error("No available objects in the freePool.");
	  }
        }
        auto obj = freePool_.back();
        freePool_.pop_back();
        return obj;
    }

    // Method to release an object back into the pool
    void release(std::shared_ptr<SocketObj> obj) {
        freePool_.push_back(obj);
    }

    // Garbage collector to clean up leaked objects
    signed int garbageCollect() {
	 signed int collected = 0;
        // For demonstration purposes, we'll print out the reference count
        std::cout << "Garbage collection in progress...\n";

        // Iterate over the pool and check the use count
        for (auto it = pool_.begin(); it != pool_.end(); ) {
            if ((*it).use_count() == 1) {
                // The only owner is the pool, so this object is not in use elsewhere
                std::cout << "Collecting the leaked object.\n";
                freePool_.push_back(*it); // Remove from pool, it will be destroyed
	        collected++;
            } else {
                ++it;
            }
        }
	return collected;
    }

private:
    std::vector<std::shared_ptr<SocketObj> > pool_; //Object pool
    std::vector<std::shared_ptr<SocketObj> > freePool_; //Free pool, from where allocation and deallocation happens
};

int main() {
    // Create a pool with 3 pre-instantiated SocketObj objects
    SocketObjPool pool(3);

    // Acquire an object from the pool
    auto obj1 = pool.acquire();
    obj1->use();

    // Acquire another object from the pool
    auto obj2 = pool.acquire();
    obj2->use();

    // Acquire another object from the pool
    auto obj3 = pool.acquire();
    obj3->use();

   try {
	 // Acquire another object from the pool
         auto obj4 = pool.acquire();
	 std::cout <<"Object memory acquired for obj4 now"<< std::endl;
         obj4->use();
   }catch (...) {
	 std::cout << "No more obj memory could be allocated" << std::endl;
   }
   pool.release(obj3);
   try {
         // Acquire another object from the pool
         auto obj4 = pool.acquire();
	 std::cout <<"Object memory acquired for obj4 now"<< std::endl;
         obj4->use();
   }catch (...) {
         std::cout << "No more obj memory could be allocated" << std::endl;
   }


    // Simulate a leak by not releasing obj1
    // Instead, we'll just let obj1 go out of scope

    // Release the second object back into the pool
    pool.release(obj2);

    // Simulate doing some work, after which we want to clean up
    // any leaked objects
    pool.garbageCollect();

    return 0;
}
