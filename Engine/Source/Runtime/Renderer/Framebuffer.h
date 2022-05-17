#pragma once

namespace Nightly
{
	class Framebuffer
	{
	public:
		Framebuffer() = default;
		~Framebuffer() = default;

		void Setup();

		void Bind() const;
		static void Unbind();

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
