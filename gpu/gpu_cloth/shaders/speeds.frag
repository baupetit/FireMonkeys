/* Speed calculation shader
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

#define SQRT2     (1.4142136)

/* num of points */
#define DELTA     (1.0/SIZE)
#define DELTA2    (LINK*DELTA)
#define GRID      (4.0/SIZE)
#define GRID_SQ2  (GRID*SQRT2)
#define GRID2     (GRID*LINK)
#define GRID2_SQ2 (GRID2*SQRT2)

/* how speed components are reduced */
#define SLOWDOWN_TANGENT 0.96
#define SLOWDOWN_NORMAL  0.92

/* how springs force is reduced */
#define REDUCTION_ALL    0.4
#define REDUCTION_ORTH   (REDUCTION_ALL*1.0)
#define REDUCTION_DIAG   (REDUCTION_ALL*0.9)
#define REDUCTION_ORTH2  (REDUCTION_ALL*0.8)
#define REDUCTION_DIAG2  (REDUCTION_ALL*0.7)
#define GRAVITY          1.0/(2.0*1024.0)

uniform sampler2D positions;
uniform sampler2D speeds;
uniform sampler2D normals;

const vec3 wind = vec3(.5,0,0);

void main()
{
    vec3 speed  = texture2D(speeds,gl_TexCoord[0].st).xyz;
    vec3 pos    = texture2D(positions,gl_TexCoord[0].st).xyz;
    vec3 normal = texture2D(normals,gl_TexCoord[0].st).xyz; /* already normalized :-) */

    speed.y -= GRAVITY;

    vec3 wn = normal * dot(wind,normal);
    speed -= 0.01 * wn;
    vec3 n = normal * dot(speed,normal);
    vec3 t = speed - n;
    speed = n*SLOWDOWN_NORMAL + t*SLOWDOWN_TANGENT;
    speed += 0.01 * wn;

    if(gl_TexCoord[0].s<1.0-DELTA)
    {
        vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(DELTA,0)).xyz - pos;
        speed += (spring - normalize(spring)*GRID) * REDUCTION_ORTH;
        //speed += spring*((1-GRID/length(spring)) * REDUCTION_ORTH);

        if(gl_TexCoord[0].t<1.0-DELTA)
        {
            vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(DELTA,DELTA)).xyz - pos;
            speed += (spring - normalize(spring)*GRID_SQ2) * REDUCTION_DIAG;
        }
        if(gl_TexCoord[0].t>DELTA)
        {
            vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(DELTA,-DELTA)).xyz - pos;
            speed += (spring - normalize(spring)*GRID_SQ2) * REDUCTION_DIAG;
        }

        if(gl_TexCoord[0].s<1.0-DELTA2)
        {
            vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(DELTA2,0)).xyz - pos;
            speed += (spring - normalize(spring)*GRID2) * REDUCTION_ORTH2;

            if(gl_TexCoord[0].t<1.0-DELTA2)
            {
                vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(DELTA2,DELTA2)).xyz - pos;
                speed += (spring - normalize(spring)*GRID2_SQ2) * REDUCTION_DIAG2;
            }
            if(gl_TexCoord[0].t>DELTA2)
            {
                vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(DELTA2,-DELTA2)).xyz - pos;
                speed += (spring - normalize(spring)*GRID2_SQ2) * REDUCTION_DIAG2;
            }
        }
    }
    if(gl_TexCoord[0].s>DELTA)
    {
        vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(-DELTA,0)).xyz - pos;
        speed += (spring - normalize(spring)*GRID) * REDUCTION_ORTH;

        if(gl_TexCoord[0].t<1.0-DELTA)
        {
            vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(-DELTA,DELTA)).xyz - pos;
            speed += (spring - normalize(spring)*GRID_SQ2) * REDUCTION_DIAG;
        }
        if(gl_TexCoord[0].t>DELTA)
        {
            vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(-DELTA,-DELTA)).xyz - pos;
            speed += (spring - normalize(spring)*GRID_SQ2) * REDUCTION_DIAG;
        }

        if(gl_TexCoord[0].s>DELTA2)
        {
            vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(-DELTA2,0)).xyz - pos;
            speed += (spring - normalize(spring)*GRID2) * REDUCTION_ORTH2;

            if(gl_TexCoord[0].t<1.0-DELTA2)
            {
                vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(-DELTA2,DELTA2)).xyz - pos;
                speed += (spring - normalize(spring)*GRID2_SQ2) * REDUCTION_DIAG2;
            }
            if(gl_TexCoord[0].t>DELTA2)
            {
                vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(-DELTA2,-DELTA2)).xyz - pos;
                speed += (spring - normalize(spring)*GRID2_SQ2) * REDUCTION_DIAG2;
            }
        }
    }
    if(gl_TexCoord[0].t<1.0-DELTA)
    {
        vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(0,DELTA)).xyz - pos;
        speed += (spring - normalize(spring)*GRID) * REDUCTION_ORTH;

        if(gl_TexCoord[0].t<1.0-DELTA2)
        {
            vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(0,DELTA2)).xyz - pos;
            speed += (spring - normalize(spring)*GRID2) * REDUCTION_ORTH2;
        }
    }
    if(gl_TexCoord[0].t>DELTA)
    {
        vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(0,-DELTA)).xyz - pos;
        speed += (spring - normalize(spring)*GRID) * REDUCTION_ORTH;

        if(gl_TexCoord[0].t>DELTA2)
        {
            vec3 spring = texture2D(positions,gl_TexCoord[0].st+vec2(0,-DELTA2)).xyz - pos;
            speed += (spring - normalize(spring)*GRID2) * REDUCTION_ORTH2;
        }
    }

    gl_FragColor = vec4(speed,0);
}
