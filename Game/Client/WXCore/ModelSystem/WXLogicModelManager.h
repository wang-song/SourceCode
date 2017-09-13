/********************************************************************
filename:   WXLogicModelManager.h

purpose:    ���ڽ���������������LogicModel��
*********************************************************************/

#ifndef _FAIRYLOGICMODELMANAGER_H_
#define _FAIRYLOGICMODELMANAGER_H_

#include <OgreSingleton.h>
#include <OgreScriptLoader.h>
#include <OgreStringVector.h>
#include <OgreDataStream.h>
#include <OgreIteratorWrappers.h>
#include <OgreResourceGroupManager.h>
#include <OgreAnimation.h>
#include <OgreMaterial.h>

// fairy header
#include "Core/WXPrerequisites.h"

namespace Ogre  {

    class CollisionManager;
    class UserCollider;
};

class TiXmlNode;

namespace WX	{

	class System;
    class TerrainData;
	class LogicModel;
    class TerrainCollider;        

	class LogicModelManager : public Ogre::Singleton<LogicModelManager>, Ogre::ScriptLoader
	{
    public:

        typedef std::map<Ogre::String, LogicModel *> LogicModelMap;

        /// �����붯��֮��Ĺ���map��keyΪ�������ƣ�valueΪ������������Ĺ����ļ�����
        typedef std::map<String, String> AnimationSkeletonLinks;
        /// keyΪlogic model�����ƣ��� Ů����.obj
        typedef std::map<String, AnimationSkeletonLinks*> AnimationSkeletonLinksMap;

        typedef unsigned long MaterialHandle;
        typedef std::list<MaterialHandle> MaterialPool;

        struct MaterialCache
        {
            MaterialCache(const Ogre::MaterialPtr& mat, MaterialPool::iterator it)
                : mMaterial(mat)
                , mPoolIterator(it)
            {
            }

            Ogre::MaterialPtr mMaterial;
            MaterialPool::iterator mPoolIterator;
        };

        typedef std::map<MaterialHandle, MaterialCache> MaterialCaches;

	public:

		LogicModelManager( WX::System *system );
		~LogicModelManager();

		/// @copydoc ScriptLoader::getScriptPatterns
		const Ogre::StringVector& getScriptPatterns(void) const;
		/// @copydoc ScriptLoader::parseScript
		void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
		/// @copydoc ScriptLoader::getLoadingOrder
		Ogre::Real getLoadingOrder(void) const;

		static LogicModelManager& getSingleton(void);       
		static LogicModelManager* getSingletonPtr(void);	

		LogicModel* createLogicModel( const String& modelName );

		LogicModel* createLogicModel( const String& name, const String& modelName );

		void destroyLogicModel(const String& name);
		void destroyLogicModel(LogicModel* model);

        /// ɾ�������Ѿ�����������LogicModel
        void destroyAllLogicModels(void);

        /// ɾ������LogicModel template
        void destroyAllLogicModelTemplates(void);

		const LogicModel * getTemplateByName(const String& modelName) const;

		LogicModel * getModelByName(const String& modelName);	

		void updateAllLogicModel(Real time);

        /// ����ȫ�ֵ��Ƿ����ͶӰ
        void setShadowCastable(bool castable);
        bool getShadowCastable(void)
        {
            return mShadowCastable;
        }

        /// ����ȫ�ֵ�������Ч����
        void setEnableWalkingEffect(bool enable);
        bool getEnableWalkingEffect(void)
        {
            return mEnableWalkingEffect;
        }

        /** ��ȡָ��obj����Դ
        @param modelName Ҫ��ȡ��obj����
        @param groupName ���ĸ���Դ���ж�ȡ��Դ�����û���ṩ�����Զ������Դ��
        @remarks ������������������ش���Logic Model��ʵ����ֻ�ǰ�
                 obj���õ���mesh����ͼ��skeleton����Դ����ȡ����
        */
        void loadModelResource(const String& modelName, 
            const String& groupName = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

        Ogre::CollisionManager* getCollisionManager(void);

        Ogre::UserCollider* getTerrainCollider(void);

        /** ��ȡָ���������ڵĹ�������
        @param modelName ģ������
        @param animName ��ָ���Ķ�������
        */
        const String& getSkeletonFromAnimationName(const String& modelName, const String& animName);

        /// ��ȡ��ָ����ģ�͵Ķ������Ƶ�����
        typedef Ogre::ConstMapIterator<AnimationSkeletonLinks> AnimationSkeletonLinksIterator;
        AnimationSkeletonLinksIterator getAnimationSkeletonLinksIterator(const String& modelName);

        /// modelģ�������
        typedef Ogre::MapIterator<LogicModelMap> LogicModelTemplateIterator;
        LogicModelTemplateIterator getLogicModelTemplateIterator(void)
        {
            return LogicModelTemplateIterator(
                mLogicModelTemplateMap.begin(), mLogicModelTemplateMap.end());
        }

        void _clearAnimationSkeletonLinks(void);

        void setAnimationInterpolationMode(Ogre::Animation::InterpolationMode mode)
        {
            mInterpolationMode = mode;
        }
        Ogre::Animation::InterpolationMode getAnimationInterpolationMode(void)
        {
            return mInterpolationMode;
        }

        void addToMaterialPool(MaterialHandle handle);

        Ogre::Entity* _createSkeletalEntity(const Ogre::SkeletonPtr& skeleton);
        void _destroySkeletalEntity(Ogre::Entity* entity);
        void _cleanupSkeletalEntityCache(void);

	protected:

		void _parseFailed( LogicModel *model, const char *errorDesc );

		void _createController(void);

		void _destroyController(void);

        /// ɾ�����õ�terrain collider
        void _destroyTerrainCollider(void);

        /// ɾ��collider manager
        void _destroyColliderManager(void);

        AnimationSkeletonLinks* _createAnimationSkeletonLinksForModel(const Ogre::String& modelName);

        /// ��������obj�Ķ���
        void _parseSingleObject(TiXmlNode* rootElement, const Ogre::String& objectName);

        void _removeMaterialPool(void);

	protected:

        AnimationSkeletonLinksMap mAnimationSkeletonLinksMap;

		Ogre::StringVector mScriptPatterns;

		WX::System *mSystem;

		LogicModelMap mLogicModelTemplateMap;
		LogicModelMap mLogicModelMap;

		ulong mAutoNamedModel;

		Ogre::Controller<Real>* mController;

        /// �Ƿ����ͶӰ
        bool mShadowCastable;

        /// �Ƿ��������Ч
        bool mEnableWalkingEffect;

        Ogre::CollisionManager* mCollisionManager;
        Ogre::UserCollider* mTerrainCollider;

        Ogre::Animation::InterpolationMode mInterpolationMode;

        MaterialPool mMaterialPool;
        MaterialCaches mMaterialCaches;

        typedef std::multimap<Ogre::Skeleton*, Ogre::Entity*> SkeletalEntityCache;
        SkeletalEntityCache mSkeletalEntityCache;
	};

};


#endif