// Copyright © 2023 GothicKit Contributors.
// SPDX-License-Identifier: MIT
#pragma once
#include <filesystem>

namespace zenkit {
	class Mmap {
	public:
		explicit Mmap(std::filesystem::path const& path);

		Mmap(Mmap const&) = delete;
		Mmap(Mmap&&) noexcept;

		~Mmap() noexcept;

		[[nodiscard]] std::byte const* data() const noexcept {
			return _m_data;
		}

		[[nodiscard]] std::size_t size() const noexcept {
			return _m_size;
		}

	private:
		std::byte const* _m_data;
		std::size_t _m_size;

		void* _m_platform_handle {nullptr};
	};
} // namespace zenkit
