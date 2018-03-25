
/**
 **	some parameters
 **/

#define	BLANK		' '
#define	DFL_SYMBOL	'*'
#define	TOP_ROW		5
#define	BOT_ROW 	20
#define	LEFT_EDGE	10
#define	RIGHT_EDGE	70
#define	X_INIT		10		/* starting col		*/
#define	Y_INIT		10		/* starting row		*/
#define	TICKS_PER_SEC	50		/* affects speed	*/

#define	X_DELAY		5
#define	Y_DELAY		8

/**
 **	the only object in town
 **/

struct ppball {
		int	x_pos, x_dir,
			y_pos, y_dir,
			y_delay, y_count,
			x_delay, x_count;
		char	symbol ;

	} ;