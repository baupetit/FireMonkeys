/* Position calculation shader
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

uniform sampler2D positions;
uniform sampler2D speeds;
uniform sampler2D normals;

const vec3 sph = vec3(-0.7,0,-0.8);
const vec3 sph2 = vec3(-0.4,0,0.5);
const vec2 cyl = vec2(1.0,0);

void main()
{
    vec3 speed = texture2D(speeds,gl_TexCoord[0].st).xyz;
    vec3 pos = texture2D(positions,gl_TexCoord[0].st).xyz;

    pos += speed*0.2;

    /* check sphere 1 collision */
    if(distance(pos,sph)<0.7)
        pos = sph + normalize(pos-sph)*0.7;

    /* check sphere 2 collision */
    if(distance(pos,sph2)<0.5)
        pos = sph2 + normalize(pos-sph2)*0.5;

    /* check cilynder collision */
    if(distance(pos.xy,cyl)<0.4)
        pos.xy = cyl + normalize(pos.xy-cyl)*0.4;

    gl_FragColor = vec4(pos,0);
}
