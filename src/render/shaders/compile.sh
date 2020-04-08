if test -f "vert.spv"; then
      rm -rf default/vert.spv
      rm -rf default/frag.spv
fi

/usr/bin/glslc shader.vert -o default/vert.spv
/usr/bin/glslc shader.frag -o default/frag.spv
