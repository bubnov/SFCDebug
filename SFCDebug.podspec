#
# Be sure to run `pod lib lint SFCDebug.podspec' to ensure this is a
# valid spec and remove all comments before submitting the spec.
#
# Any lines starting with a # are optional, but encouraged
#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name            = "SFCDebug"
  s.version         = "0.1.0"
  s.summary         = "Wrapper for the CocoaLamberjack logging framework"
  s.homepage        = "https://github.com/bubnov/SFCDebug"
  s.license         = 'MIT'
  s.author          = { "Bubnov Slavik" => "bubnovslavik@gmail.com" }
  s.source          = { :git => "https://github.com/bubnov/SFCDebug.git",
		                :tag => s.version.to_s }
  s.platform        = :ios, '7.0'
  s.requires_arc    = true
  s.source_files    = 'SFCDebug'

  # s.public_header_files = 'Pod/Classes/**/*.h'
  # s.frameworks = 'UIKit', 'MapKit'
  s.dependency 'CocoaLumberjack', '~> 1.9'
end
