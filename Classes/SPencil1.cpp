#include"SPencil1.h"

///---------------------------------------
// 
// SPencil1
// 
///---------------------------------------
enum
{
	SIZE_X = 256,
	SIZE_Y = 256,
};

SPencil1::SPencil1()
:_center(Vec2(0.0f, 0.0f))
, _resolution(Vec2(0.0f, 0.0f))
, _time(0.0f)
{
	zoneposlen = 0;

}

SPencil1::~SPencil1()
{
}

SPencil1* SPencil1::SPencil1WithVertex(const std::string &vert, const std::string& frag)
{
	auto node = new (std::nothrow) SPencil1();
	node->initWithVertex(vert, frag);
	node->autorelease();

	return node;
}

bool SPencil1::initWithVertex(const std::string &vert, const std::string &frag)
{
	log("initWithVertex:%s",vert.c_str());
//#if CC_ENABLE_CACHE_TEXTURE_DATA
//	log("CC_ENABLE_CACHE_TEXTURE_DATA");
//	auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
//		this->setGLProgramState(nullptr);
//		loadShaderVertex(_vertFileName, _fragFileName);
//	});
//
//	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//#endif
	log("initWithVertex1");
	_vertFileName = vert;
	_fragFileName = frag;

	loadShaderVertex(vert, frag);
	log("initWithVertex2");
	auto size = Director::sharedDirector()->getWinSize();
	//float w = size, h = SIZE_Y;  //可视窗口，view的大小
	float w = size.width;
	float h = size.height;

	_time = 0;
	mx = 0;
	my = 0;
	//setmousexy(0,0);
	//影响：glsl 的 iResolution,类似camera是视距(远近，看到画面内容多少也会不同)
	//_resolution = Vec2(SIZE_X, SIZE_Y);
	_resolution = Vec2(w, h);
	getGLProgramState()->setUniformVec2("resolution", _resolution);
	getGLProgramState()->setUniformVec2("mouse", Vec2(mx,my));
	//left用来发送texture,发送后删除,这个是用来做笔刷的
	auto left = Sprite::create("Images/pencel2.png"); //noise grossinis_sister2 ,elephant1_Diffuse.png,noise,hcf,powered pencel1
	addChild(left, 0, 10);
	left->setPosition(150, 150);
	getGLProgramState()->setUniformTexture("u_texture1", left->getTexture());
	getGLProgramState()->setUniformVec2("u_texture1Size", Vec2(left->getContentSize().width, left->getContentSize().height));
	left->removeFromParent();

	scheduleUpdate();

	setContentSize(Size(SIZE_X, SIZE_Y));
	setAnchorPoint(Vec2(0.5f, 0.5f));
	log("initWithVertex3");
	return true;
}

void SPencil1::loadShaderVertex(const std::string &vert, const std::string &frag)
{
	auto fileUtiles = FileUtils::getInstance();

	// frag
	auto fragmentFilePath = fileUtiles->fullPathForFilename(frag);
	auto fragSource = fileUtiles->getStringFromFile(fragmentFilePath);

	// vert
	std::string vertSource;
	if (vert.empty()) {
		vertSource = ccPositionTextureColor_vert;
	}
	else {
		std::string vertexFilePath = fileUtiles->fullPathForFilename(vert);
		vertSource = fileUtiles->getStringFromFile(vertexFilePath);
	}
	//log("glprogram:%s,%s", vertSource.c_str(), fragSource.c_str());
	auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
	log("glprogram1");  
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	log("glprogram2");//o9dfdsf
	setGLProgramState(glprogramstate);
	log("glprogram3");//o9dfdsf
}

void SPencil1::update(float dt)
{
	_time += dt;
}

void SPencil1::setPosition(const Vec2 &newPosition)
{
	Node::setPosition(newPosition);
	auto position = getPosition();
	_center = Vec2(position.x * CC_CONTENT_SCALE_FACTOR(), position.y * CC_CONTENT_SCALE_FACTOR());
	getGLProgramState()->setUniformVec2("center", _center);
}

void SPencil1::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder, transform, flags);
	_customCommand.func = CC_CALLBACK_0(SPencil1::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void SPencil1::onDraw(const Mat4 &transform, uint32_t flags)
{
	log("SPencil1::onDraw");
	auto size = Director::sharedDirector()->getWinSize();
		//float w = size, h = SIZE_Y;  //可视窗口，view的大小
	float w = size.width;
	float h = size.height;
	//每2个float表示一个点pos，3组pos表示一个三角形，一共2个三角形
	GLfloat vertices[12] = { 0, 0, w, 0, w, h, 0, 0, 0, h, w, h }; //影响：shader窗口大小（shader画的内容占cocos视窗的大小比例，看到内容本身不变化）

	auto glProgramState = getGLProgramState();
	glProgramState->setVertexAttribPointer("a_position", 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glProgramState->setUniformVec2("mouse",Vec2(mx,my));
	float hw = size.width/2;
	float hh = size.height/2;
	log("w,h:%f,%f",w,h);
	//60,130 190,130
	glProgramState->apply(transform);

	//往shader传送 pos数组
	//这个放在 glProgramState->apply(transform); 之前就会报错,可能_glprogram->use(); 执行顺序有关
	//GLfloat pos[7] = { 0, 60, 160, 150, 160 ,180,130};
	//const int len = 10;
	//GLfloat pos[len] = { -0.041600, 0.437600, -0.039520, 0.440720, -0.035360, 0.446960, -0.033280, 0.450080, -0.029120, 0.456320 };
	//const int len = 250;
	//GLfloat pos[len] = { 0.53, 0.37, 0.54, 0.34, 0.55, 0.31, 0.56, 0.29, 0.56, 0.26, 0.57, 0.23, 0.58, 0.20, 0.59, 0.17, 0.59, 0.14, 0.59, 0.11, 0.60, 0.08, 0.60, 0.05, 0.60, 0.02, 0.60, -0.02, 0.59, -0.05, 0.58, -0.07, 0.57, -0.10, 0.56, -0.13, 0.55, -0.16, 0.54, -0.18, 0.52, -0.20, 0.51, -0.22, 0.49, -0.24, 0.47, -0.25, 0.45, -0.26, 0.43, -0.28, 0.41, -0.28, 0.39, -0.29, 0.37, -0.29, 0.35, -0.30, 0.33, -0.30, 0.31, -0.31, 0.29, -0.31, 0.27, -0.32, 0.25, -0.32, 0.23, -0.32, 0.21, -0.32, 0.19, -0.32, 0.17, -0.32, 0.15, -0.32, 0.13, -0.31, 0.11, -0.30, 0.09, -0.29, 0.07, -0.27, 0.06, -0.24, 0.05, -0.22, 0.03, -0.19, 0.02, -0.17, 0.01, -0.14, 0.01, -0.11, -0.00, -0.08, -0.01, -0.05, -0.02, -0.02, -0.03, 0.01, -0.04, 0.03, -0.04, 0.06, -0.05, 0.09, -0.05, 0.12, -0.06, 0.15, -0.06, 0.18, -0.06, 0.22, -0.06, 0.25, -0.06, 0.28, -0.06, 0.29, -0.08, 0.29, -0.10, 0.30, -0.12, 0.30, -0.15, 0.30, -0.17, 0.30, -0.19, 0.30, -0.21, 0.29, -0.23, 0.28, -0.25, 0.27, -0.27, 0.26, -0.28, 0.24, -0.30, 0.23, -0.32, 0.22, -0.34, 0.21, -0.36, 0.20, -0.38, 0.18, -0.40, 0.17, -0.41, 0.15, -0.43, 0.12, -0.44, 0.10, -0.45, 0.07, -0.46, 0.04, -0.47, 0.02, -0.48, -0.01, -0.50, -0.03, -0.51, -0.05, -0.53, -0.07, -0.55, -0.08, -0.57, -0.10, -0.58, -0.11, -0.60, -0.13, -0.62, -0.15, -0.62, -0.18, -0.62, -0.22, -0.61, -0.24, -0.60, -0.27, -0.58, -0.28, -0.56, -0.30, -0.54, -0.31, -0.52, -0.32, -0.50, -0.32, -0.48, -0.33, -0.46, -0.34, -0.44, -0.34, -0.42, -0.34, -0.40, -0.34, -0.38, -0.34, -0.36, -0.34, -0.34, -0.34, -0.32, -0.34, -0.29, -0.34, -0.27, -0.35, -0.25, -0.35, -0.23, -0.35, -0.21, -0.35, -0.19, -0.36, -0.17, -0.36, -0.15, -0.37, -0.13, -0.37, -0.11, -0.38, -0.09, -0.39 };
	
	auto glProgram = getGLProgramState()->getGLProgram();
	int len = pushidx;
	//glProgram->setUniformLocationWith2fv((GLint)glProgram->getUniformLocationForName("pos"), pos, len/2);  //vec2 数组？
	//fix 4 len
	int flen = 4;
	if(len >=4){
		GLfloat tpos[4] = {0,0,0,0};
		tpos[0] = pos[len - 4];
		tpos[1] = pos[len - 3];
		tpos[2] = pos[len - 2];
		tpos[3] = pos[len - 1];
		//只传送最后4个点
		//glProgram->setUniformLocationWith1i((GLint)glProgram->getUniformLocationForName("poslen"), flen);
		//glProgram->setUniformLocationWith1fv((GLint)glProgram->getUniformLocationForName("pos"), tpos, flen);  // float 数组

		//传送全部点
		glProgram->setUniformLocationWith1i((GLint)glProgram->getUniformLocationForName("poslen"), len);
		glProgram->setUniformLocationWith1fv((GLint)glProgram->getUniformLocationForName("pos"), pos, len);  // float 数组
	}
	else
	{
		//传送全部点
		glProgram->setUniformLocationWith1i((GLint)glProgram->getUniformLocationForName("poslen"), len);
		glProgram->setUniformLocationWith1fv((GLint)glProgram->getUniformLocationForName("pos"), pos, len);  // float 数组
	}
	log("SPencil1::onDraw poslen:%d", len);
	//color
	//brushColorF,测试用
	//if (brushColorF.equals(Color4F::BLUE))
	//{
	//	brushColorF = Color4F::RED;
	//}
	//else if (brushColorF.equals(Color4F::RED))
	//{
	//	brushColorF = Color4F::YELLOW;
	//}
	//else if (brushColorF.equals(Color4F::YELLOW))
	//{
	//	brushColorF = Color4F::BLUE;
	//}
	glProgram->setUniformLocationWith4f((GLint)glProgram->getUniformLocationForName("scolor"), brushColorF.r, brushColorF.g, brushColorF.b, brushColorF.a); //test 0.5
	log("SPencil1::onDraw scolor");
//#ifdef ZoneCode
	//zone部分
	//glProgram->setUniformLocationWith1i((GLint)glProgram->getUniformLocationForName("zoneposlen"), zoneposlen);
	//glProgram->setUniformLocationWith1fv((GLint)glProgram->getUniformLocationForName("zonepos"), zonepos, zoneposlen * 2);  // float 数组
	log("SPencil1::onDraw zoneposlen:%d", zoneposlen);
//#endif
	/*
	std::string str = "{";
	for (int i = 0; i < len;i++)
	{
		str += CCString::createWithFormat("%.2f", pos[i])->getCString();
		if (i != len - 1)
		{
			str += ",";
		}
	}
	str += "};";
	str += CCString::createWithFormat("%d", len)->getCString();
	//log("uniform float pos[] = %s",str.c_str());
	*/
	glDrawArrays(GL_TRIANGLES, 0, 6);
	log("SPencil1::onDraw GL_TRIANGLES");
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
	CHECK_GL_ERROR_DEBUG();
	log("SPencil1::CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES");
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#ifdef OnlySendOneTime
	//渲染完成一次，清除点
	this->clearAllMouseXY();
#endif
	log("SPencil1::onDraw end all");
}

void SPencil1::pushmousexy(float mx_, float my_)
{
	auto size = Director::sharedDirector()->getWinSize();
	float hw = size.width / 2;
	float hh = size.height / 2;
	float x = (mx_ - hw) / hw;
	float y = (my_ - hh) / hh;
	//log("pushidx x:,%d,%f", pushidx, x);
	pos[pushidx++] = x;
	//log("pushidx y:,%d,%f,%f,%f,%f", pushidx, x, y, mx_, my_);
	pos[pushidx++] = y;
	//x,y范围是[-1,1]
	//log("pushidx,x,y:,%d,%f,%f", pushidx, x, y);
	//log("pushmousexy shader pos len:%d", pushidx);
	
}

void SPencil1::setzonepos(const std::vector<Vec2>& param)
{
	zoneposlen = param.size();
	if (zoneposlen > 3)
	{
		for (int i = 0; i < param.size(); i++)
		{
			zonepos[i * 2] = param[i].x;
			zonepos[i * 2 + 1] = param[i].y;
		}

		/*
		//zone部分
		auto glProgram = getGLProgramState()->getGLProgram();
		glProgram->setUniformLocationWith1i((GLint)glProgram->getUniformLocationForName("zoneposlen"), zoneposlen);
		glProgram->setUniformLocationWith1fv((GLint)glProgram->getUniformLocationForName("zonepos"), zonepos, zoneposlen * 2);  // float 数组
		*/
	}
	
}

void SPencil1::setShaderTexture(const std::string& name, Texture2D* texture)
{
	//auto glProgram = getGLProgramState()->getGLProgram();
	getGLProgramState()->setUniformTexture(name, texture);
}

void SPencil1::clearAllMouseXY()
{
	pushidx = 0;
}

void SPencil1::setmousexys(std::deque<Vec2>& pool)
{
	while (pool.size() > 0)
	{
		Vec2 temp = pool.at(0);
		pushmousexy(temp.x, temp.y);
		pool.pop_front();
	}
}