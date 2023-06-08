/*
 * Copyright (C) 2020, Inria
 * GRAPHDECO research group, https://team.inria.fr/graphdeco
 * All rights reserved.
 *
 * This software is free for non-commercial, research and evaluation use 
 * under the terms of the LICENSE.md file.
 *
 * For inquiries contact sibr@inria.fr and/or George.Drettakis@inria.fr
 */
#pragma once

# include "Config.hpp"
# include <core/renderer/RenderMaskHolder.hpp>
# include <core/scene/BasicIBRScene.hpp>
# include <core/system/SimpleTimer.hpp>
# include <core/system/Config.hpp>
# include <core/graphics/Mesh.hpp>
# include <core/view/ViewBase.hpp>
# include <core/renderer/CopyRenderer.hpp>
# include <core/renderer/PointBasedRenderer.hpp>
# include <atomic>
# include <mutex>
# include <memory>
# include <core/graphics/Texture.hpp>
#include <projects/remote/json.hpp>
using json = nlohmann::json;

namespace sibr { 

	/**
	 * \class RemotePointView
	 * \brief Wrap a ULR renderer with additional parameters and information.
	 */
	class SIBR_EXP_ULR_EXPORT RemotePointView : public sibr::ViewBase
	{
		SIBR_CLASS_PTR(RemotePointView);

	public:

		/**
		 * Constructor
		 * \param ibrScene The scene to use for rendering.
		 * \param render_w rendering width
		 * \param render_h rendering height
		 */
		RemotePointView(const sibr::BasicIBRScene::Ptr& ibrScene, uint render_w, uint render_h);

		/** Replace the current scene.
		 *\param newScene the new scene to render */
		void setScene(const sibr::BasicIBRScene::Ptr & newScene);

		/**
		 * Perform rendering. Called by the view manager or rendering mode.
		 * \param dst The destination rendertarget.
		 * \param eye The novel viewpoint.
		 */
		void onRenderIBR(sibr::IRenderTarget& dst, const sibr::Camera& eye) override;

		/**
		 * Update the GUI.
		 */
		void onGUI() override;

		/** \return a reference to the scene */
		const std::shared_ptr<sibr::BasicIBRScene> & getScene() const { return _scene; }

		virtual ~RemotePointView() override;

	protected:

		struct RemoteRenderInfo
		{
			Vector2i imgResolution;
			float fovy;
			float fovx;
			float znear;
			float zfar;
			Matrix4f view;
			Matrix4f viewProj;
		};

		RemoteRenderInfo _remoteInfo;

		bool _doTrainingBool = true;
		bool _doSHsPython = false;
		bool _doRotScalePython = false;
		bool _skipValidation = false;
		bool _keepAlive = true;
		bool _showSfM = false;

		float _scalingModifier = 1.0f;

		std::atomic<bool> keep_running = true;

		void send_receive();

		GLuint _imageTexture;

		bool _renderSfMInMotion = false;

		bool _imageResize = true;
		bool _imageDirty = true;
		uint32_t _timestampRequested = 1;
		uint32_t _timestampReceived = 0;

		std::mutex _renderDataMutex;
		std::mutex _imageDataMutex;

		std::unique_ptr <std::thread> _networkThread;
		std::vector<unsigned char> _imageData;

		std::shared_ptr<sibr::BasicIBRScene> _scene; ///< The current scene.
		PointBasedRenderer::Ptr _pointbasedrenderer;
		CopyRenderer::Ptr _copyRenderer;
	};

} /*namespace sibr*/ 
