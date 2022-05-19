# Copyright (C) 2022 Dominik Chat
#
# This file is part of Waves.
#
# Waves is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version 3
# of the License, or (at your option) any later version.
#
# Waves is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Waves. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.9)
project(waves)

include(CheckIPOSupported)

add_executable(waves main.c idft.c)

check_ipo_supported(RESULT result)
if(result)
	set_property(TARGET waves PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_link_libraries(waves -lglut -lGLU -lGL -lm)
target_compile_options(waves PRIVATE -O2 -m64 -Wall -Wextra -Werror)

install(TARGETS waves RUNTIME DESTINATION bin)
