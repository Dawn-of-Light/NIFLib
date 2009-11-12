Notes from Mashani:

Maik is still updating this code, so changes/improvements will occur over time.

That said, if anyone updates the C++ library please update the example framework as appropriate so that c# users can take advantage of your changes.

I will be implementing a customized version of the c# implementation in the XmlDbEditor.  Once it's done you will be able to grab it from the XmlDbEditor CVS and see if it meets your needs better then the one here.

You will need to download the NVIDIA SDK and have the OpenGL headers in your include path for the C++ library code (NIFLib) to compile.   You can get that here:
http://developer.nvidia.com/object/sdk_home.html
--maik's comment: only the 6 header files from "/gl" and "glh/" are needed. Have to check out the redist. rules.

Note that it's a 199 meg download... but I don't know for a fact that redistributing the necessary files is allowed, so I didn't add them to the CVS.

Read "whats that.txt" and "docu.txt" for some notes from Maik.  

Have fun.
