/****************************************
Mr.Yiu 
进料功能集
****************************************/
#ifndef _FEED_H_
#define	_FEED_H_

	#define	FeedVacuo_Stop		Q_Set(Q_FeedVacuo1,ON);Q_Set(Q_FeedVacuo2,ON);Q_Set(Q_FeedVacuo3,ON);
	#define	FeedVacuo_Wait		Q_Set(Q_FeedVacuo1,OFF);Q_Set(Q_FeedVacuo2,OFF);Q_Set(Q_FeedVacuo3,OFF);
	#define	FeedVacuo_Inp		Q_Set(Q_FeedVacuo1,ON);Q_Set(Q_FeedVacuo2,ON);Q_Set(Q_FeedVacuo3,ON);
	#define	FeedVacuo_Take		Q_Set(Q_FeedVacuo1,OFF);Q_Set(Q_FeedVacuo2,OFF);Q_Set(Q_FeedVacuo3,ON);
	#define	FeedVacuo_PreOff	Q_Set(Q_FeedVacuo1,OFF);Q_Set(Q_FeedVacuo2,OFF);Q_Set(Q_FeedVacuo3,OFF);
	
	extern void Feed(void);

#endif

