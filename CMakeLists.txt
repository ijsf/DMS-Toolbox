# -----------------------------------------------------------------------------
# - Basic project configuration                                               -
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.12)

project(DMS-Toolbox)
set(VERSION 0.1)

# -----------------------------------------------------------------------------
# - Documentation (not maintained for Windows, should be built on Linux)      -
# -----------------------------------------------------------------------------
find_package(Doxygen)

if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "doxygen-doc")

    add_custom_command(
        COMMENT "Generate HTML/LaTeX documentation"
        OUTPUT doxygen-doc/latex/index.tex
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        VERBATIM
    )

    add_custom_command(
        COMMENT "Generate PDF documentation"
        DEPENDS doxygen-doc/latex/index.tex
        OUTPUT doxygen-doc/latex/refman.pdf
        WORKING_DIRECTORY doxygen-doc/latex
        COMMAND rm -f *.aux *.toc *.idx *.ind *.ilg *.log *.out
        COMMAND pdflatex refman.tex || true
        COMMAND makeindex refman.idx
        COMMAND pdflatex refman.tex || true
        COMMAND egrep "'Rerun (LaTeX|to get cross-references right)'" refman.log >/dev/null 2>&1 && pdflatex refman.tex || true
        COMMAND egrep "'Rerun (LaTeX|to get cross-references right)'" refman.log >/dev/null 2>&1 && pdflatex refman.tex || true
        COMMAND egrep "'Rerun (LaTeX|to get cross-references right)'" refman.log >/dev/null 2>&1 && pdflatex refman.tex || true
        COMMAND egrep "'Rerun (LaTeX|to get cross-references right)'" refman.log >/dev/null 2>&1 && pdflatex refman.tex || true
        COMMAND egrep "'Rerun (LaTeX|to get cross-references right)'" refman.log >/dev/null 2>&1 && pdflatex refman.tex || true
    )

    add_custom_target(doc
        DEPENDS doxygen-doc/latex/refman.pdf
        WORKING_DIRECTORY doxygen-doc
        COMMAND mv -f latex/refman.pdf dmstoolbox.pdf
    )
endif(DOXYGEN_FOUND)

# -----------------------------------------------------------------------------
# - Binaries                                                                  -
# -----------------------------------------------------------------------------
add_subdirectory(src)
