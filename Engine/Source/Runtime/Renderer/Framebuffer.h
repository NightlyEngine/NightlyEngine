#pragma once

namespace Nightly
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
	{
	public:
		Framebuffer() = default;
		~Framebuffer() = default;

		void Setup();

		// Recreates the framebuffer using the new resolution.
		void Invalidate(int width, int height);

		void Bind() const;
		static void Unbind();

		void Cleanup();

		[[nodiscard]] uint32_t GetColorBuffer() const
		{
			return m_ColorBuffer;
		}

	private:
		uint32_t m_Framebuffer;
		uint32_t m_ColorBuffer;
		uint32_t m_RenderBuffer;
	};
}
