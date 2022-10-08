# Documentation

- Doxygen is the documentation toolset that we use to document Petari's code onto a web page for easy viewing. Doxygen also uses Graphviz for graphics for displaying inheritance.

### Installing
- Follow the instructions that coorespond to your OS on Doxygen's **[installation]** page. If you want as well, you can install **[GraphViz]**, but this is optional. If you choose to not install GraphViz, set `HAVE_DOT` in the `Doxyfile` to `NO`.

### Running
Once everything has been installed, just run `doxygen` in the root directory and it will generate HTML files in a new folder named `doxygen`. Verify that the output meets standards before pull requesting documented headers / code! Do not worry about submitting any form of the HTML, the documentation server will pull the repository every 15 minutes and then compile the documents itself **[here]**. 

[installation]: https://doxygen.nl/manual/install.html
[GraphViz]: https://graphviz.org/download/
[here]: http://shibbo.net/smg/docs/Petari/doxygen/