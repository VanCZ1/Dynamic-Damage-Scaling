# Dynamic-Damage-Scaling
An SKSE plugin that allows users to customize the damage scaling.
Built with [CommonLibSSE-NG](https://github.com/alandtse/CommonLibVR).

# Requirements
* [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
	* Desktop development with C++
* [CMake](https://cmake.org/)
	* Add to your `PATH`
* [vcpkg](https://github.com/microsoft/vcpkg)
	1. Clone the repository
	2. Run `bootstrap-vcpkg.bat` in the `vcpkg` folder
	3. Add a system environment variable:
		* Name: `VCPKG_ROOT`
		* Value: path to your `vcpkg` folder

# User Requirements
* [Address Library for SKSE](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
	* Needed for SSE
* [VR Address Library for SKSEVR](https://www.nexusmods.com/skyrimspecialedition/mods/58101)
	* Needed for VR

# Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

# Building
```
git clone https://github.com/VanCZ1/Dynamic-Damage-Scaling.git
cd Dynamic-Damage-Scaling
git submodule update --init --recursive
```

```
cmake --preset release-msvc
cmake --build --preset release-msvc
```
