#include "xyjvideoplayer.h"

xyjVideoPlayer::xyjVideoPlayer(QWidget *parent)
	: QWidget(parent)
{
	this->init_ui();
	this->init_videoPlayer();
	this->init_connections();
	this->init_table_menu();
	setWindowFlags(/*Qt::Window |*/Qt::FramelessWindowHint);
	//setWindowOpacity(0.9);
	setAttribute(Qt::WA_TranslucentBackground,true);
	QWidgetResizeHandler *movein=new QWidgetResizeHandler(this);
	movein->setMovingEnabled(true);
	this->setFixedSize(QSize(800,400));
	this->raise_button();

	this->_xyjPath=QApplication::applicationDirPath();
	this->read_list();
}

xyjVideoPlayer::~xyjVideoPlayer()
{

}
void xyjVideoPlayer::paintEvent(QPaintEvent* event){
	QPainter p(this);
	p.drawPixmap(0,0,800,400,QPixmap(":/xyjVideoPlayer/Resources/full1.png"));
	p.drawPixmap(0,30,800,300,QPixmap(":/xyjVideoPlayer/Resources/full2.png"));
}
void xyjVideoPlayer::dragEnterEvent(QDragEnterEvent *event){
	event->acceptProposedAction();
}
void xyjVideoPlayer::dropEvent(QDropEvent *event){
	QList<QUrl> urls=event->mimeData()->urls();
	if(urls.isEmpty()) return;
	QStringList filelist;
	foreach(QUrl u,urls){
		QString file=u.toLocalFile();
		filelist<<file;
	}
	this->add_list(filelist);
}
//void xyjVideoPlayer::mouseDoubleClickEvent(QMouseEvent* event){
//	if(!this->_mediaPlayer->isVideoAvailable()) return;
//	if(!this->_videoWidget->isFullScreen()){
//		this->_videoWidget->setFullScreen(true);
//	}else{
//		this->_videoWidget->setFullScreen(false);
//	}
//}
void xyjVideoPlayer::init_ui(){
	
	this->_add_video_button=new xyjButton(this);
	QIcon normal_add,focus_add;
	normal_add.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/search_hover.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_add.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/search.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_add_video_button->set_icons(normal_add,focus_add,focus_add);
	this->_add_video_button->setIconSize(QSize(25,25));
	this->_add_video_button->setGeometry(QRect(20,350,35,35));
	
	this->_add_video_button->setFocusPolicy(Qt::NoFocus);
	this->_add_video_button->setFlat(true);
	this->_add_video_button->setAttribute(Qt::WA_TranslucentBackground);

	this->_play_button=new xyjButton(this);
	QIcon normal_play,focus_play;
	normal_play.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/pause_hover.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_play.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_play_button->set_icons(normal_play,focus_play,focus_play);
	this->_play_button->setIconSize(QSize(35,35));
	this->_play_button->setGeometry(QRect(265+100,340,55,55));
	this->_play_button->setFocusPolicy(Qt::NoFocus);
	_play_button->setFlat(true);
	this->_play_button->setAttribute(Qt::WA_TranslucentBackground);

	this->_pause_button=new xyjButton(this);
	QIcon normal_pause,focus_pause;
	normal_pause.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/play_hover.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_pause.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/play.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_pause_button->set_icons(normal_pause,focus_pause,focus_pause);
	this->_pause_button->setIconSize(QSize(35,35));
	this->_pause_button->setGeometry(QRect(265+100,340,55,55));
	this->_pause_button->setFocusPolicy(Qt::NoFocus);
	this->_pause_button->setVisible(false);
	_pause_button->setFlat(true);
	this->_pause_button->setAttribute(Qt::WA_TranslucentBackground);

	this->_next_button=new xyjButton(this);
	QIcon normal_next,focus_next;
	normal_next.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/next_hover.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_next.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/next.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_next_button->set_icons(normal_next,focus_next,focus_next);
	this->_next_button->setIconSize(QSize(30,30));
	this->_next_button->setGeometry(QRect(350+100,340,55,55));
	this->_next_button->setFocusPolicy(Qt::NoFocus);
	_next_button->setFlat(true);
	this->_next_button->setAttribute(Qt::WA_TranslucentBackground);

	this->_previous_button=new xyjButton(this);
	QIcon normal_pre,focus_pre;
	normal_pre.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/last_hover.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_pre.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/last.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_previous_button->set_icons(normal_pre,focus_pre,focus_pre);
	this->_previous_button->setIconSize(QSize(30,30));
	this->_previous_button->setGeometry(QRect(175+100,340,55,55));
	this->_previous_button->setFocusPolicy(Qt::NoFocus);
	_previous_button->setFlat(true);
	this->_previous_button->setAttribute(Qt::WA_TranslucentBackground);

	this->_play_table_button=new xyjButton(this);
	QIcon normal_table,focus_table;
	normal_table.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/playlist.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_table.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/playlist_hover.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_play_table_button->set_icons(normal_table,focus_table,focus_table);
	this->_play_table_button->setIconSize(QSize(35,35));
	this->_play_table_button->setGeometry(QRect(80,350,35,35/*730,350,35,35*/));//20,350,35,35
	this->_play_table_button->setFocusPolicy(Qt::NoFocus);
	this->_play_table_button->setFlat(true);
	this->_play_table_button->setAttribute(Qt::WA_TranslucentBackground);


	this->_close_button=new xyjButton(this);
	QIcon normal_close,focus_close;
	normal_close.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/关闭按钮.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_close.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/关闭按钮2.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_close_button->set_icons(normal_close,focus_close,focus_close);
	this->_close_button->setIconSize(QSize(15,15));
	this->_close_button->setGeometry(QRect(770,5,25,25));
	this->_close_button->setFocusPolicy(Qt::NoFocus);
	this->_close_button->setFlat(true);
	this->_close_button->setAttribute(Qt::WA_TranslucentBackground);


	this->_min_button=new xyjButton(this);
	QIcon normal_min,focus_min;
	normal_min.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/缩小按钮.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_min.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/缩小按钮2.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_min_button->set_icons(normal_min,focus_min,focus_min);
	this->_min_button->setIconSize(QSize(15,15));
	this->_min_button->setGeometry(QRect(740,5,25,25));
	this->_min_button->setFocusPolicy(Qt::NoFocus);
	this->_min_button->setFlat(true);
	this->_min_button->setAttribute(Qt::WA_TranslucentBackground);

	this->_vol_yes_button=new xyjButton(this);
	QIcon normal_yes,focus_yes;
	normal_yes.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/音量图标.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_yes.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/音量图标2.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_vol_yes_button->set_icons(normal_yes,focus_yes,focus_yes);
	this->_vol_yes_button->setIconSize(QSize(30,30));
	this->_vol_yes_button->setGeometry(QRect(550,355,25,25));
	this->_vol_yes_button->setFocusPolicy(Qt::NoFocus);
	this->_vol_yes_button->setFlat(true);
	this->_vol_yes_button->setAttribute(Qt::WA_TranslucentBackground);

	this->_vol_no_button=new xyjButton(this);
	QIcon normal_no,focus_no;
	normal_no.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/静音图标.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_no.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/静音图标2.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_vol_no_button->set_icons(normal_no,focus_no,focus_no);
	this->_vol_no_button->setIconSize(QSize(30,30));
	this->_vol_no_button->setGeometry(QRect(550,355,25,25));
	this->_vol_no_button->setFocusPolicy(Qt::NoFocus);
	this->_vol_no_button->setVisible(false);
	this->_vol_no_button->setFlat(true);
	this->_vol_no_button->setAttribute(Qt::WA_TranslucentBackground);

	this->_vol_slider=new QSlider(Qt::Horizontal,this);
	this->_vol_slider->setGeometry(QRect(580,363,100,10));
	this->_vol_slider->setEnabled(true);
	this->_vol_slider->setValue(100);
	this->_vol_slider->setStyleSheet(
		"QSlider::groove:horizontal{border:0px;height:4px;}"  
		"QSlider::sub-page:horizontal{background:red;}"  
		"QSlider::add-page:horizontal{background:black;} "
		"QSlider::handle:horizontal{background:white;width:10px;border:#51b5fb 10px;border-radius:5px;margin:-3px 0px -3px 0px;}");

	this->_repeat_button=new xyjButton(this);
	QIcon normal_r,focus_r;
	normal_r.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/repeat.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_r.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/repeat.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_repeat_button->set_icons(normal_r,focus_r,focus_r);
	this->_repeat_button->setIconSize(QSize(20,20));
	this->_repeat_button->setGeometry(QRect(200,355,25,25));
	this->_repeat_button->setFocusPolicy(Qt::NoFocus);
	this->_repeat_button->setVisible(true);
	this->_repeat_button->setFlat(true);
	this->_repeat_button->setAttribute(Qt::WA_TranslucentBackground);

	this->_single_repeat_button=new xyjButton(this);
	QIcon normal_sr,focus_sr;
	normal_sr.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/single_repeat.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_sr.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/single_repeat.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_single_repeat_button->set_icons(normal_sr,focus_sr,focus_sr);
	this->_single_repeat_button->setIconSize(QSize(20,20));
	this->_single_repeat_button->setGeometry(QRect(200,355,25,25));
	this->_single_repeat_button->setFocusPolicy(Qt::NoFocus);
	this->_single_repeat_button->setVisible(false);
	this->_single_repeat_button->setFlat(true);
	this->_single_repeat_button->setAttribute(Qt::WA_TranslucentBackground);

	this->_random_button=new xyjButton(this);
	QIcon normal_ran,focus_ran;
	normal_ran.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/random.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_ran.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/random.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_random_button->set_icons(normal_ran,focus_ran,focus_ran);
	this->_random_button->setIconSize(QSize(20,20));
	this->_random_button->setGeometry(QRect(200,355,25,25));
	this->_random_button->setFocusPolicy(Qt::NoFocus);
	this->_random_button->setVisible(false);
	this->_random_button->setFlat(true);
	this->_random_button->setAttribute(Qt::WA_TranslucentBackground);

	this->_full_screen_button=new xyjButton(this);
	QIcon normal_full,focus_full;
	normal_full.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/fullscreen_hover.png"), QSize(), QIcon::Normal, QIcon::Off);
	focus_full.addFile(QStringLiteral(":/xyjVideoPlayer/Resources/fullscreen.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->_full_screen_button->set_icons(normal_full,focus_full,focus_full);
	this->_full_screen_button->setIconSize(QSize(20,20));
	this->_full_screen_button->setGeometry(QRect(730,355,25,25));//580,360,100,10
	this->_full_screen_button->setFocusPolicy(Qt::NoFocus);
	this->_full_screen_button->setVisible(true);
	this->_full_screen_button->setFlat(true);
	this->_full_screen_button->setAttribute(Qt::WA_TranslucentBackground);

/*************************************************************************************/
	this->_playTable=new QTableWidget(this);
	this->_playTable->setGeometry(QRect(0,30,300,300/*500,30,300,300*/));
	this->_playTable->setItemDelegate(new NoFocusDelegate());
	
	this->_playTable->setVisible(false);

	this->_playTable->setColumnCount(2);
	this->_playTable->setColumnWidth(0,230);
	this->_playTable->setColumnWidth(1,70);
	//使得可以更新表格状态
	QTableWidgetItem *qtablewidgetitem=new QTableWidgetItem();
	this->_playTable->setHorizontalHeaderItem(0,qtablewidgetitem);
	//禁止编辑
	this->_playTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//选中一行
	this->_playTable->setSelectionBehavior(QAbstractItemView::SelectRows);	
	//去除水平滚动条
	this->_playTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->_playTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
	//去表头
	this->_playTable->horizontalHeader()->setFixedHeight(0);
	this->_playTable->verticalHeader()->setFixedWidth(0);
	//设置无边框
	this->_playTable->setFrameShape(QFrame::NoFrame);
	//设置不显示单元格分界线
	this->_playTable->setShowGrid(false);
	//this->_playTable->setMouseTracking(true);
	
	this->_playTable->setStyleSheet("QTableWidget::item:selected { background-color: rgb(150, 150, 150) }"
									"QTableWidget { background-color: rgb(100, 100, 100) }"
									"QScrollBar{background:transparent; width: 0px;}"
									/*"QScrollBar::sub-line{background:transparent;}"
									"QScrollBar::add-line{background:transparent;}"
									"QScrollBar::handle{background:black; border:2px solid transparent; border-radius:5px;}"
									"QScrollBar::handle:hover{background:red;}"*/);
	this->_playTable->setContextMenuPolicy(Qt::CustomContextMenu);
	//this->_playTable->setStyleSheet("QScrollBar{background:transparent; width: 5px;}"
	//	"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
	//	"QScrollBar::handle:hover{background:gray;}"
	//	"QScrollBar::sub-line{background:transparent;}"
	//	"QScrollBar::add-line{background:transparent;}");
	this->_playTable->setAcceptDrops(true);
	this->setAcceptDrops(true);
	/*************************************************************/
	this->_play_process_slider=new xyjSlider(Qt::Horizontal,this);
	this->_play_process_slider->setGeometry(QRect(50,330,700,10));
	this->_play_process_slider->setStyleSheet(
		"QSlider::groove:horizontal{border:0px;height:4px;}"  
		"QSlider::sub-page:horizontal{background:red;}"  
		"QSlider::add-page:horizontal{background:black;} "
		"QSlider::handle:horizontal{background:white;width:10px;border:#51b5fb 10px;border-radius:5px;margin:-3px 0px -3px 0px;}"
		);
	this->_play_process_slider->setEnabled(false);//初始设置为无法移动
}
void xyjVideoPlayer::raise_button(){
	this->_add_video_button->raise();
	this->_play_button->raise();
	this->_vol_no_button->raise();
	this->_vol_yes_button->raise();
	this->_pause_button->raise();
	this->_next_button->raise();
	this->_previous_button->raise();
	this->_close_button->raise();
	this->_min_button->raise();
	this->_play_table_button->raise();
	this->_playTable->raise();
	this->_single_repeat_button->raise();
	this->_repeat_button->raise();
	this->_random_button->raise();
	this->_full_screen_button->raise();
}
void xyjVideoPlayer::init_videoPlayer(){
	_mediaPlayer = new QMediaPlayer(this);
	_playList = new QMediaPlaylist(this);
	_mediaPlayer->setPlaylist(_playList);
	_playList->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
	this->_play_mode=PLAYMODE::LISTCIRCLE;

	_videoWidget = new xyjVideoWidget(this,_mediaPlayer);
	_videoWidget->setGeometry(QRect(0,30,800,300));//视频窗口
	
	_mediaPlayer->setVideoOutput(_videoWidget);
}

void xyjVideoPlayer::init_connections(){
	connect(_mediaPlayer,SIGNAL(durationChanged(qint64)),this,SLOT(update_duration(qint64)));
	connect(_mediaPlayer,SIGNAL(positionChanged(qint64)),this,SLOT(update_position(qint64)));
	connect(_play_process_slider,SIGNAL(sliderMoved(int)),this,SLOT(set_media_play_position(int)));/////////////
	connect(_play_process_slider,SIGNAL(sliderReleased()),this,SLOT(slider_released_play()));
	//connect(_play_process_slider,SIGNAL(()),this,SLOT(slider_pressed_play()));
	//_play_process_slider->slider
	connect(_add_video_button,SIGNAL(clicked()),this,SLOT(open_video()));
	connect(_play_button,SIGNAL(clicked()),this,SLOT(play_button_clicked()));
	connect(_vol_slider,SIGNAL(valueChanged(int)),_mediaPlayer,SLOT(setVolume(int)));
	connect(_vol_yes_button,SIGNAL(clicked()),this,SLOT(vol_yes_button_clicked()));
	connect(_vol_no_button,SIGNAL(clicked()),this,SLOT(vol_no_button_clicked()));
	connect(_pause_button,SIGNAL(clicked()),this,SLOT(pause_button_clicked()));
	connect(_next_button,SIGNAL(clicked()),this,SLOT(next_button_clicked()));
	connect(_previous_button,SIGNAL(clicked()),this,SLOT(pre_button_clicked()));
	connect(_close_button,SIGNAL(clicked()),this,SLOT(close_button_clicked()));
	connect(_min_button,SIGNAL(clicked()),this,SLOT(min_button_clicked()));
	connect(_full_screen_button,SIGNAL(clicked()),this,SLOT(fullscreen_button_clicked()));
	connect(_play_table_button,SIGNAL(clicked()),this,SLOT(play_table_button_clicked()));

	connect(_playTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(play_table_double_clicked(int,int)));
	//connect(_playTable,SIGNAL(cellEntered(int ,int)),this,SLOT(mouse_enter_cell(int, int)));
	connect(_playTable,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(play_table_menu_requested(const QPoint&)));
	
	connect(_repeat_button,SIGNAL(clicked()),this,SLOT(play_mode_button_clicked()));
	connect(_single_repeat_button,SIGNAL(clicked()),this,SLOT(play_mode_button_clicked()));
	connect(_random_button,SIGNAL(clicked()),this,SLOT(play_mode_button_clicked()));
}
void xyjVideoPlayer::init_table_menu(){
	this->_menu_on_table_text=new QMenu(_playTable);
	this->_menu_on_table_blank=new QMenu(_playTable);

	this->_add_video_action=new QAction(QString::fromLocal8Bit("添加视频"),this);
	this->_remove_cur_action=new QAction(QString::fromLocal8Bit("移除所选视频"),this);
	this->_remove_all_action=new QAction(QString::fromLocal8Bit("移除所有视频"),this);

	connect(_add_video_action,SIGNAL(triggered()),this,SLOT(open_video()));
	connect(_remove_cur_action,SIGNAL(triggered()),this,SLOT(remove_current_video()));
	connect(_remove_all_action,SIGNAL(triggered()),this,SLOT(remove_all_video()));


	this->_menu_on_table_text->addAction(_add_video_action);
	this->_menu_on_table_text->addAction(_remove_cur_action);
	this->_menu_on_table_text->addAction(_remove_all_action);

	
	this->_menu_on_table_blank->addAction(_add_video_action);
	this->_menu_on_table_blank->addAction(_remove_all_action);
}

/*********************************************/
void xyjVideoPlayer::update_duration(qint64 duration){
	this->_play_process_slider->setRange(0,duration);//进度条范围调到和进度相同
	this->_play_process_slider->setEnabled(true);//////////
	//this->_play_process_slider->setPageStep(duration/10);
}
void xyjVideoPlayer::update_position(qint64 position){
	if(!this->_play_process_slider->isSliderDown()){//当滑动条没被按住
		_play_process_slider->setValue(position);
	}
}
void xyjVideoPlayer::set_media_play_position(int pos){
	this->_play_position=pos;
}
qint64 xyjVideoPlayer::get_media_play_positon(){
	return this->_play_position;
}
void xyjVideoPlayer::slider_released_play(){
	this->_mediaPlayer->setPosition(get_media_play_positon());
	this->_mediaPlayer->play();
}
void xyjVideoPlayer::slider_pressed_play(){
	this->_mediaPlayer->setPosition(get_media_play_positon());
	this->_mediaPlayer->play();
}
void xyjVideoPlayer::open_video(){
	QStringList fileList = QFileDialog::getOpenFileNames(this,QString::fromLocal8Bit("add video"),"c:/",QString("MP4(*.mp4)"));
	this->add_list(fileList);
}
void xyjVideoPlayer::play_button_clicked(){
	if(this->_playList->isEmpty()) return;
	if(this->_mediaPlayer->state()==QMediaPlayer::PausedState){
		this->_mediaPlayer->play();
		this->_pause_button->setVisible(true);
		this->_play_button->setVisible(false);
		return;
	}
	if(this->_playTable->selectedItems().isEmpty()){
		play_table_double_clicked(0,0);
		return;
	}
	this->_playList->setCurrentIndex(_playTable->selectedItems().begin().i->t()->row());

	play_table_double_clicked(_playList->currentIndex(),0);
}
void xyjVideoPlayer::vol_yes_button_clicked(){
	this->_cur_volume=this->_mediaPlayer->volume();
	this->_mediaPlayer->setVolume(0);
	this->_vol_slider->setValue(0);
	this->_vol_yes_button->setVisible(false);
	this->_vol_no_button->setVisible(true);
}
void xyjVideoPlayer::vol_no_button_clicked(){
	this->_mediaPlayer->setVolume(_cur_volume);
	this->_vol_slider->setValue(_cur_volume);
	this->_vol_yes_button->setVisible(true);
	this->_vol_no_button->setVisible(false);
}
void xyjVideoPlayer::pause_button_clicked(){
	_mediaPlayer->pause();
	this->_pause_button->setVisible(false);
	this->_play_button->setVisible(true);
}
void xyjVideoPlayer::next_button_clicked(){//默认末尾的下一首是第一首，无需调整
	this->_playList->next();
	if(_mediaPlayer->state()==QMediaPlayer::State::PlayingState){//播放状态下按下一首才有效
		this->_mediaPlayer->play();
	}
}
void xyjVideoPlayer::pre_button_clicked(){//第一首的上一首不是最后一首，所以需要调整一下
	int cur=_playList->currentIndex();
	int pre=_playList->previousIndex();
	if(pre>=0) this->_playList->previous();
	else this->_playList->setCurrentIndex(_fileList.size()-1);
	if(_mediaPlayer->state()==QMediaPlayer::State::PlayingState) {
		this->_mediaPlayer->play();
	}
}
void xyjVideoPlayer::close_button_clicked(){
	this->close();
}
void xyjVideoPlayer::min_button_clicked(){
	this->showMinimized();
}
void xyjVideoPlayer::play_table_button_clicked(){
	if(!this->_playTable->isVisible()){
		if(this->_mediaPlayer->isAudioAvailable()){
			this->_videoWidget->setGeometry(QRect(300,30,500,300));//视频为列表腾出空间
		}
		this->_playTable->setVisible(true);
	}else{
		this->_playTable->setVisible(false);
		if(this->_mediaPlayer->isAudioAvailable()){
			this->_videoWidget->setGeometry(QRect(0,30,800,300));//视频窗口恢复原位
		}
	}
}
void xyjVideoPlayer::play_table_double_clicked(int row,int){
	QFile file(_fileList.at(row));
	if(!file.open(QIODevice::ReadOnly)){
		_playTable->item(row,1)->setText(QString::fromLocal8Bit("无效视频"));
		return;
	}
	file.close();
	_playList->setCurrentIndex(row);
	_mediaPlayer->play();
	this->_videoWidget->setGeometry(QRect(0,30,800,300));//视频窗口恢复原位
	this->_playTable->setVisible(false);
	this->_pause_button->setVisible(true);
	this->_play_button->setVisible(false);
}
void xyjVideoPlayer::play_table_menu_requested(const QPoint& pos){
	if(this->_playTable->itemAt(pos)!=NULL){
		this->_select_row_index=this->_playTable->itemAt(pos)->row();//获取所选行号
		this->_menu_on_table_text->exec(QCursor::pos());
	}else{
		this->_menu_on_table_blank->exec(QCursor::pos());
	}
}
void xyjVideoPlayer::play_mode_button_clicked(){
	if(this->_play_mode==PLAYMODE::LISTCIRCLE){
		this->_play_mode=PLAYMODE::SINGLEREPEAT;
		this->_single_repeat_button->setVisible(true);
		this->_repeat_button->setVisible(false);
		this->_playList->setPlaybackMode(QMediaPlaylist::PlaybackMode::CurrentItemInLoop);
		//todo
	}else if(this->_play_mode==PLAYMODE::SINGLEREPEAT){
		this->_play_mode=PLAYMODE::RANDOM;
		this->_single_repeat_button->setVisible(false);
		this->_random_button->setVisible(true);
		this->_playList->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random);
	}else{
		this->_play_mode=PLAYMODE::LISTCIRCLE;
		this->_random_button->setVisible(false);
		this->_repeat_button->setVisible(true);
		this->_playList->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
	}
}
void xyjVideoPlayer::fullscreen_button_clicked(){
	//未播放时不能全屏
	//qt assitant上说“If available, the QVideoWidget class can be used to view the video”
	if(!this->_mediaPlayer->isVideoAvailable()) return;
	else this->_videoWidget->setFullScreen(true);
}

void xyjVideoPlayer::mouse_enter_cell(int row,int col){
	int rows=this->_playTable->rowCount();
	int cols=this->_playTable->columnCount();
	for(int i=0;i<rows;i++){
		if(i==row){
			for(int j=0;j<cols;j++){
				this->_playTable->itemAt(i,j)->setBackgroundColor(QColor(255,255,0));
			}
		}else{
			for(int j=0;j<cols;j++){
				if(i%2==0){
					this->_playTable->item(i,j)->setBackgroundColor(QColor(250,250,250));
				}
				else{
					this->_playTable->item(i,j)->setBackgroundColor(QColor(200,200,200));
				}
			}
		}
	}
}
/***************************************/
void xyjVideoPlayer::add_list(QStringList fileList){
	foreach(QString filename,fileList){
		if(this->_fileList.contains(filename)) continue;
		this->_fileList<<filename;
		this->_playList->addMedia(QUrl::fromLocalFile(filename));
		this->_playTable->insertRow(_playTable->rowCount());

		for(int i=0;i<_playTable->columnCount();i++){
			QTableWidgetItem* item=new QTableWidgetItem;
			item->setFont(QFont(QString::fromLocal8Bit("微软雅黑")));
			_playTable->setItem(_playTable->rowCount()-1,i,item);
		}
		this->_playTable->item(_playTable->rowCount()-1,0)->setText(QFileInfo(filename).baseName());
	}
	this->save_list();
	this->paint_table();
}
void xyjVideoPlayer::remove_all_video(){
	this->_mediaPlayer->stop();
	this->_playList->clear();
	this->_fileList.clear();
	while(this->_playTable->rowCount()){
		this->_playTable->removeRow(0);
	}
	this->_playTable->clear();
	this->save_list();
	this->paint_table();
}
void xyjVideoPlayer::remove_current_video(){
	this->_mediaPlayer->stop();
	this->_fileList.removeAt(this->_select_row_index);
	this->_playList->removeMedia(this->_select_row_index);
	this->_playTable->removeRow(this->_select_row_index);
	this->paint_table();
	this->save_list();
}
void xyjVideoPlayer::save_list(){
	QFile file(this->_xyjPath+"xyjVideoList.txt");
	if(!file.open(QIODevice::WriteOnly)) return;

	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_5_5);
	QStringList fileList=this->_fileList;
	stream<<fileList;

	file.close();
}
void xyjVideoPlayer::read_list(){
	QFile file(this->_xyjPath+"xyjVideoList.txt");
	if(!file.open(QIODevice::ReadOnly)) return;

	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_5_5);
	QStringList fileList;
	stream>>fileList;
	this->add_list(fileList);

	file.close();
}
void xyjVideoPlayer::paint_table(){
	for(int i=0;i<_playTable->rowCount();i++){
		for(int j=0;j<_playTable->columnCount();j++){
			if(i%2==0){
				this->_playTable->item(i,j)->setBackgroundColor(QColor(120,120,120));
			}else{
				this->_playTable->item(i,j)->setBackgroundColor(QColor(100,100,100));
			}
		}
	}
}