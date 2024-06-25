# Teardown Multiplayer

## Things to note
A lot of this code has a chance of being refactored. As it is being developed, you might find some messy code, commented out code, testing stuff, etc...  
So nothing will be final is essentially what I'm saying (like h_lua_newstate being in dllmain.cc (as of 25/07/24), this will be changed)

## Requirements
- [Premake](https://premake.github.io/)
- [Visual Studio 2022](https://visualstudio.microsoft.com/)
- [VCPKG](https://vcpkg.io/en/)
  - vcpkg install argparse:x64-windows-static
  - vcpkg install minhook:x64-windows-static

# For Developers

## How to compile
```
1. Run `premake5 vs2022` in your command line
2. Open the generated solution file in Visual Studio
3. Build the solution
```

## Contributing Guide
We welcome contributions to TDMP! In order to contribute towards the project, you must follow these steps:

### 1. Fork The Repository and Clone it
Since you cannot directly contribute to this repository, you will need to create a fork  
Once forked, you must clone it to your machine

### 2. Create a Branch
When creating a branch, you want to keep it simple. For example: `impl-thirdperson` or `fix-minimize-crash`.  
You can create the branch by doing the following: `git checkout -b name`

### 3. Make & Commit Your Changes
After you have made your changes, you will need to test them, make sure everything works as expected. This might require someone else to help test it with you.

Once you have successfully tested your changes, you will want to commit by doing the following:
```
git add .
git commit -m "Description of changes"
```

Make sure not to include any extra files left over (maybe your editor produces a config file for example, don't include that.)

### 4. Create the Pull Request
Now that you have made your changes, you can go to the repository (either) and create a pull request there.  
You will want to give it a good title and a concise description of what you are changing.

You will then have to wait for a response or for it to get merged, we might ask you to change something before we can merge it.

### Guidelines
- Ensure your code follows the project's coding standards.
- Write clear, concise commit messages.
- Update documentation if your changes affect it.
- Be respectful and considerate in your communications.

## TODO for This Repository
- Create a `.clang-format` file to help contributers and us keep a consistent style