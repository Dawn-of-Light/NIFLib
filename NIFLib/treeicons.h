
// Image list
#define TIC_NOTCONNECTED	0
#define TIC_ABSTRACT		1
#define TIC_CLASS			2
#define TIC_DATA			3
#define TIC_CHILDS			4
#define TIC_TRANSPOSE		5
#define TIC_PARTICLE		6
#define TIC_TEXTURE			7
#define TIC_CONTROLLER		8
#define TIC_MESH			9
#define TIC_LINK			10
#define TIC_SKIN			11
#define TIC_PROPERTY		12

// combined with
// - TICS_FINISHED_RENDERING_MISSING will not be shown right now
#define TICS_INVESTIGATE				(0<<16)		// node must be checked
#define TICS_FINISHED					(1<<16)		// node completly known
#define TICS_FINISHED_RENDERING_MISSING	(2<<16)		// node completly known but not or just partitial rendered
#define TICS_UNKNOWNDATA				(3<<16)		// still unknown data
#define TICS_UNKNOWNDATA_LISTED			(4<<16)		// unknown data but is listed in object tree
#define TICS_UNKNOWNDATA_NOT_CRITICAL	(1<<16)	//(5<<16)		// unknown data but is listed in object tree


// Overlays - these are just th
// - note: TICST_INVESTIGATE + TICST_FINISHED are swaped. So every 'just icon' will have an INVESTIGATE icon
#define TICST_FINISHED					0 // just no overlay
#define TICST_INVESTIGATE				1
#define TICST_UNKNOWNDATA				2
#define TICST_UNKNOWNDATA_LISTED		3
#define TICST_UNKNOWNDATA_NOT_CRITICAL	4


