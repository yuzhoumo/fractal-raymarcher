#version 330 core

#define MAX_STEPS 100
#define MAX_DIST 100.
#define SURF_DIST .01

out vec4 o_frag_color;

uniform vec3 u_camera_pos;
uniform vec3 u_camera_front;
uniform vec3 u_camera_up;
uniform vec3 u_camera_right;
uniform float u_fov;
uniform float u_time;
uniform vec2 u_resolution;

float get_dist(vec3 p) {
  vec4 sphere = vec4(0, 1, 6, 1);
  float sphere_dist = length(p - sphere.xyz) - sphere.w;
  float plane_dist = p.y;
  return min(sphere_dist, plane_dist);
}

float ray_march(vec3 ro, vec3 rd) {
  float dO = 0.0;

  for(int i = 0; i < MAX_STEPS; i++) {
    vec3 p = ro + rd * dO;
    float dS = get_dist(p);
    dO += dS;
    if (dO > MAX_DIST || dS < SURF_DIST) break;
  }

  return dO;
}

vec3 get_normal(vec3 p) {
  float d = get_dist(p);
  vec2 e = vec2(0.01, 0);

  vec3 n = d - vec3(get_dist(p - e.xyy),
                    get_dist(p - e.yxy),
                    get_dist(p - e.yyx));

  return normalize(n);
}

float get_light(vec3 p) {
  vec3 light_pos = vec3(0, 5, 6);
  light_pos.xz += vec2(sin(u_time), cos(u_time))*2.;
  vec3 l = normalize(light_pos - p);
  vec3 n = get_normal(p);

  float dif = clamp(dot(n, l), 0.0, 1.0);
  float d = ray_march(p + n * SURF_DIST * 2.0, l);
  if (d < length(light_pos - p)) dif *= 0.1;

  return dif;
}

void main() {
  vec3 color = vec3(0);

  float aspect_ratio = u_resolution.x / u_resolution.y;
  vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution.xy) / u_resolution.y;
  vec3 ro = u_camera_pos;

  float scale = tan(radians(u_fov * 0.5));
  vec3 rd = normalize(
    u_camera_right * uv.x * scale * aspect_ratio + u_camera_up * uv.y * scale + u_camera_front);

  float d = ray_march(ro, rd);
  vec3 p = ro + rd * d;
  float dif = get_light(p);

  color = vec3(dif);
  color = pow(color, vec3(0.4545)); // gamma correction

  o_frag_color = vec4(color, 1.0);
}
