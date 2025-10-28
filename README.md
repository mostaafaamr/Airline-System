- Designed and implemented a modular airline reservation system following OOP principles (Encapsulation, Inheritance, and
Polymorphism).
- Developed classes for User, Administrator, Flight, Aircraft, Reservation, and Maintenance Logs with proper relationships
and data abstraction.
- Implemented JSON-based persistent storage for users, flights, reservations, and aircraft information.
- Added robust features like user management, flight scheduling, reservation booking/cancellation, and maintenance log
tracking.
- Employed modern C++ features: smart pointers, STL containers, exception handling, and RAII for resource safety.



You must install vcpkg:
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh        # on Linux/macOS
.\bootstrap-vcpkg.bat       # on Windows
cd ..
vcpkg/vcpkg install --manifest
