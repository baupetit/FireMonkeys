/* Normal calculation shader
 *
 * Copyright (C) 2005, Maurizio Monge <maurizio.monge@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define DELTA  (1.0/SIZE)

uniform sampler2D positions;
uniform sampler2D speeds;
uniform sampler2D normals;

void main()
{
    vec3 v1 = texture2D(positions,gl_TexCoord[0].st+vec2(DELTA,0)).xyz
            - texture2D(positions,gl_TexCoord[0].st+vec2(-DELTA,0)).xyz;
    vec3 v2 = texture2D(positions,gl_TexCoord[0].st+vec2(0,DELTA)).xyz
            - texture2D(positions,gl_TexCoord[0].st+vec2(0,-DELTA)).xyz;

    gl_FragColor = vec4(-normalize(cross(v1,v2)),0);
}
