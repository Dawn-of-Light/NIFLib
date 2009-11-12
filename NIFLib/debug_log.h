#pragma once

// create an own name space
//  to keep 'global funktion tab' in visualstudio clean
namespace debug
{

	#ifdef _DEBUG
	extern void	_initlog();
	extern void	_endlog();
	extern void	_protocol_windows();
	extern void	_protocol_windows_init();
	extern void	_protocol_windows_end();
	extern void	_logout(const char*file, int line, char*,...);
	extern void	_logoutfile(char*,...);


	#define INITLOG()				debug::_initlog()
	#define ENDLOG()				debug::_endlog()
	#define LOG(x)					debug::_logout(__FILE__,__LINE__,x)
	#define LOG1(x,a)				debug::_logout(__FILE__,__LINE__,x,a)
	#define LOG2(x,a,b)				debug::_logout(__FILE__,__LINE__,x,a,b)
	#define LOG3(x,a,b,c)			debug::_logout(__FILE__,__LINE__,x,a,b,c)
	#define LOG4(x,a,b,c,d)			debug::_logout(__FILE__,__LINE__,x,a,b,c,d)
	#define LOG5(x,a,b,c,d,e)		debug::_logout(__FILE__,__LINE__,x,a,b,c,d,e)
	#define LOG6(x,a,b,c,d,e,f)		debug::_logout(__FILE__,__LINE__,x,a,b,c,d,e,f)


	#undef ASSERT
	#define ASSERT(x)				{if (!(x)) {LOG1("ASSERT: %s",#x);DebugBreak();}}

	#define ASSERTE(x,y)			{if (!(x)) {LOG1("ASSERT: %s",y);ASSERT(x);};}

	#undef TRACE
	#define TRACE(x)					debug::_logout(__FILE__,__LINE__,x)
	#undef TRACE0
	#define TRACE0(x)					debug::_logout(__FILE__,__LINE__,x)
	#undef TRACE1
	#define TRACE1(x,y)					debug::_logout(__FILE__,__LINE__,x,y)
	#undef TRACE2
	#define TRACE2(x,y,z)				debug::_logout(__FILE__,__LINE__,x,y,z)
	#undef TRACE3
	#define TRACE3(x,a,b,c)				debug::_logout(__FILE__,__LINE__,x,a,b,c)


	#else

	#define INITLOG()
	#define ENDLOG()
	#define LOG(x)					
	#define LOG1(x,a)				
	#define LOG2(x,a,b)				
	#define LOG3(x,a,b,c)			
	#define LOG4(x,a,b,c,d)	
	#define LOG5(x,a,b,c,d,e)	
	#define LOG6(x,a,b,c,d,e,f)	

	#undef ASSERT
	#define ASSERT(x)	

	#undef TRACE
	#define TRACE(x)	
	#undef TRACE0
	#define TRACE0(x)		
	#undef TRACE1
	#define TRACE1(x,y)		
	#undef TRACE2
	#define TRACE2(x,y,z)	
	#undef TRACE3
	#define TRACE3(x,a,b,c)	

	#define ASSERTE(x,y)

	#endif

}
