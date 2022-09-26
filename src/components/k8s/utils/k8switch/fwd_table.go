/*
 * k8switch API
 *
 * k8switch API generated from k8switch.yang
 *
 * API version: 1.0.0
 * Generated by: Swagger Codegen (https://github.com/swagger-api/swagger-codegen.git)
 */

package swagger

type FwdTable struct {
	// Destination IP address
	Address string `json:"address,omitempty"`
	// Destination MAC address
	Mac string `json:"mac,omitempty"`
	// Port where this entry is present
	Port string `json:"port,omitempty"`
}
