Pod::Spec.new do |s|
  s.name            = "SFCDebug"
  s.version         = "0.1.0"
  s.summary         = "Wrapper for the CocoaLamberjack logging framework"
  s.homepage        = "https://github.com/bubnov/SFCDebug"
  s.license         = 'MIT'
  s.author          = { "Bubnov Slavik" => "bubnovslavik@gmail.com" }
  s.source          = { :git => "https://github.com/bubnov/SFCDebug.git", :tag => s.version.to_s }
  s.platform        = :ios, '7.0'
  s.requires_arc    = true
  s.source_files    = 'SFCDebug'
  s.dependency 'CocoaLumberjack', '~> 1.9'
end
