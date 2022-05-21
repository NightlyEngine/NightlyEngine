#pragma once

namespace Nightly
{
	class Framebuffer
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
