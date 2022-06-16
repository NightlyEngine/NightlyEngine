#pragma once

#include "Core/EngineAPI.h"
#include "Core/Core.h"

namespace NL
{
	struct NL_API FramebufferProps
	{
		FramebufferProps() = default;

		FramebufferProps(int width, int height, bool hdr)
				: m_Width(width), m_Height(height), m_HDR(hdr)
		{
		}

		NL_NODISCARD int GetWidth() const
		{
			return m_Width;
		}

		NL_NODISCARD int GetHeight() const
		{
			return m_Height;
		}

		NL_NODISCARD bool IsHDR() const
		{
			return m_HDR;
		}

	private:
		int m_Width = 1280;
		int m_Height = 720;
		bool m_HDR = false;

		friend class Framebuffer;
	};

	class NL_API Framebuffer
	{
	public:
		Framebuffer()
				: m_Framebuffer(), m_ColorBuffer(), m_RenderBuffer(), m_Props()
		{
		}

		explicit Framebuffer(const FramebufferProps& props)
				: m_Framebuffer(), m_ColorBuffer(), m_RenderBuffer(), m_Props(props)
		{
			Generate(props);
		}

		~Framebuffer() = default;

		// Recreates the framebuffer using the new properties.
		void Invalidate(const FramebufferProps& props);

		void Bind() const;
		static void Unbind();

		void Cleanup();

		NL_NODISCARD uint32_t GetColorBuffer() const
		{
			return m_ColorBuffer;
		}

		NL_NODISCARD FramebufferProps GetProps() const
		{
			return m_Props;
		}

	private:
		void Generate(const FramebufferProps& props);

		uint32_t m_Framebuffer;
		uint32_t m_ColorBuffer;
		uint32_t m_RenderBuffer;

		FramebufferProps m_Props;
	};
}
