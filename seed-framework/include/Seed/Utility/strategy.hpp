/*
* Copyright (c) <2015> <Antoine Richard and Jérémy Riffet, S.E.E.D>
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

/*!
 * \file strategy.hpp
 * \author Antoine "Avzgui" Richard
 * \date August 24, 2015
 * \copyright Copyright (c) 2015,
 * Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
 * \license Zlib License.
 */

#ifndef STRATEGY_HPP
#define STRATEGY_HPP

/*!
 * \class Strategy strategy.hpp Seed/Utility/strategy.hpp
 * 
 * \brief Abstract class used to implement strategy classes in the
 * framework's parts.
 */
class Strategy {

	public :

		///Constructor
		Strategy();

		///Abstract execute method
		virtual void execute() = 0;

		///Destructor
		~Strategy();
};

#endif //STRATEGY_HPP