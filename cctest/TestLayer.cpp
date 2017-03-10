
#include"TestLayer.h"
#include "../utils/Utility.h"
#include <vector>

bool TestLayer::init()
{
	CCLayer::init();
	
	auto vec = std::vector<const char*>();
	//char* names[] = { "Skill/skill_1101_h.png", "Skill/skill_13_s_l.png", "Skill/skill_13_s_r.png", "Skill/skill_13_s.png", "Skill/skill_13_h.png", "Skill/skill_21_h.png"};
	char* names[] = { "Skill/skill_1101_h.png"};
	auto len = sizeof(names) / sizeof(char*);
	log("getCachedTextureInfo: %s", TextureCache::getInstance()->getCachedTextureInfo().c_str());
	for (auto item : names)
	{
		//vec.push_back("Skill/skill_1101_h.png", "Skill/skill_1101_h.png");
		auto timer1 = utils::getTimeInMilliseconds();
		auto skill = TextureCache::getInstance()->addImage(item);
		auto sprite = Sprite::createWithTexture(skill);
		log("%s spend:%lld", item, utils::getTimeInMilliseconds() - timer1);
		addChild(sprite);
	}
	log("getCachedTextureInfo end: %s", TextureCache::getInstance()->getCachedTextureInfo().c_str());
	
	//log("names:%d,%d,%d", sizeof(names), sizeof(names[0]), sizeof(names)/ sizeof(names[0]));
	
	return true;
}

TestLayer::~TestLayer()
{
	
}




