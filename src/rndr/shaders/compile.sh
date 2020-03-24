if test -f "vert.spv"; then
      rm -rf vert.spv
      rm -rf frag.spv
fi

/usr/bin/glslc shader.vert -o vert.spv
/usr/bin/glslc shader.frag -o frag.spv
