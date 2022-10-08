
# Build Instructions

<br>

1. **Obtain** a clean copy of `main.dol` from **Super Mario Galaxy 1 (Korean)**

<br>

2. **Rename** it to `baserom.dol`

<br>

3. **Place** it on the root folder

<br>

4. **Download** / **Install** dependencies with:
    
    ```sh
    python setup.py
    ```
    
<br>
    
5. **Build** with:

    ```sh
    python build.py
    ```
    
<br>
    
6. **Verify** your build with:

    ```sh
    python check.py symbol_name
    ```

    If your function is matching, then it will return with 0 errors. Be careful, as paired-single instructions are currently unsupported so those are ignored.

<br>

7. **Update** your progress with:

    ```sh
    python progress.py
    ```

    This will update all of the functions, documents, and the progress badge.

<br>   
    
*For more information check the **[Contribution Guide]** .*


<!----------------------------------------------------------------------------->

[Contribution Guide]: CONTRIBUTING.md
